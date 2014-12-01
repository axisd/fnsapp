#include "gnivc.h"

// System
#include <stdint.h>

// Main
#include "liblogger/cdebug.h"

// Qt
#include <QFile>
#include <QEventLoop>
#include <QTimer>

/*#ifdef __linux__
    struct termios new_settings;
    struct termios stored_settings;
#endif*/

//const double Mult = 0.1;


CGnivcSender::CGnivcSender(QObject *parent) :
    QObject(parent)
{}

CGnivcSender::~CGnivcSender()
{}

void CGnivcSender::ClearToken(const QString& __fileName)
{
    QFile file(__fileName);

	QByteArray content;

	file.open(QIODevice::ReadOnly);
	content = file.readAll();
    file.close();

	QList<QByteArray> iniList(content.split(0x0A));

	content.clear();
	for (int i = 0; i < iniList.size(); ++i)
	{
		if (iniList[i].contains("token") || iniList[i].isEmpty())
        {
			iniList[i].clear();
        }
		else
        {
			content +=iniList[i] + '\x0A';
        }
	}

	file.open(QIODevice::WriteOnly);
	file.write(content);
	file.close();
}

bool CGnivcSender::initLibrary(const QString& __iniFile) 
{
	libvpm::initLogger(false, "./");

	m_instance = NULL;

    libvpm::Settings settings;
    settings = libvpm::SettingsLoader::loadFromFile(__iniFile.toStdString(), true);

    bool ok(false);

    m_instance = new libvpm::Instance(settings);
    m_instance->start();

    procEvent(1000);

    switch(m_instance->mode())
    {
        /** Нормальный */
        case MODE_NORMAL:
        {
            LOG_MESSAGE(logger::t_info, "main",
                        QString("Запущено в нормальном режиме"));
            ok = true;
            break;
        }

        /** Автономный */
        case MODE_RESCUE:
        {
            LOG_MESSAGE(logger::t_info, "main",
                        QString("Запущено в автономном режиме"));
            ok = true;
            break;
        }

        /** Заблокирован (из-за неправильного токена) */
        case MODE_BLOCKED_INVALID_TOKEN:
        {
            LOG_MESSAGE(logger::t_fatal, "main",
                        QString("Заблокирован (из-за неправильного токена)"));
            ok = false;
            break;
        }

        /** Не пройдена начальная инициализация */
        case MODE_NOT_INITIALIZED:
        {
            LOG_MESSAGE(logger::t_fatal, "main",
                        QString("Не пройдена начальная инициализация"));
            ok = false;
            break;
        }

        /** Заблокирован */
        case MODE_BLOCKED:
        {
            LOG_MESSAGE(logger::t_fatal, "main",
                        QString("Заблокирован"));
            ok = false;
            break;
        }

       /** Заблокирован (превышено время нахождения в аварийном режиме)
         *  (считается от первого аварийного чека).
         */
        case MODE_BLOCKED_RESCUE_PERIOD_EXCEEDED:
        {
            LOG_MESSAGE(logger::t_fatal, "main",
                        QString("Заблокирован (превышено время нахождения в аварийном режиме)"));
            ok = false;
            break;
        }

        default:
        {
            LOG_MESSAGE(logger::t_fatal, "main",
                        QString("Неизвестное состояние"));
            ok = false;
            break;
        }
    }

    if(!ok)
    {
        m_instance->stop();
        delete (m_instance);
        m_instance = NULL;
    }
    else
    {
        ClearToken(__iniFile);
    }

    return ok;
}

void CGnivcSender::StopInstance(void)
{
	if (m_instance != NULL)
    {
		m_instance->stop();
    }
}

void CGnivcSender::SendReceipt(const SSCO::ReceiptV1Ptr __receipt, QString& __fiscalText)
{
	if (m_instance == NULL)
    {
		throw(std::runtime_error("Библиотека libvpm не запущена. Данные отправляться не будут."));
    }

	vpmOperation_t op;
    switch (__receipt->m_type)
	{
    case SSCO::ModelReceiptV1::HEADER_SALE :
    case SSCO::ModelReceiptV1::HEADER_POP :
		op = VPM_OPERATION_SELL;
		break;

    case SSCO::ModelReceiptV1::HEADER_RETURN :
    case SSCO::ModelReceiptV1::HEADER_RETURN_BY_RECEIPT :
		op = VPM_OPERATION_SELL_RETURN;
        break;

	default :
		DEBUG_ASSERT(false);
	};

    QDateTime epoch(QDate(1970,1,1), QTime(0,0,0));
    unsigned int secs = epoch.secsTo(__receipt->m_open_datetime);

    LOG_MESSAGE(logger::t_info, "main",
                tr("Receipt time: DateTime(%1) Secs(%2)")
                .arg(__receipt->m_open_datetime.toString("ddMMyyyy hhmmss"))
                .arg(secs));

	libvpm::Ticket ticket(VPM_DOMAIN_TRADING, // nothrow
		op,
		secs,
        __receipt->m_login_id,
        __receipt->m_login_name.toStdString());

    QVector<SSCO::ReceiptItemV1Ptr> normItems = __receipt->m_items;

	for (int i = 0; i < normItems.size(); ++i)
	{
		vpmStorno_t iType = VPM_ITEM_REGULAR;
        if (normItems[i]->m_type == SSCO::ModelReceiptItemV1::ITEM_VOID)
        {
			iType = VPM_ITEM_STORNO;
        }
        if (normItems[i]->m_type == SSCO::ModelReceiptItemV1::ITEM_CANCEL)
        {
			continue;
        }

		libvpm::Commodity commodity(iType,		// nothrow
            normItems[i]->m_name.toStdString(),
            __receipt->m_stock_name.toStdString(),
            normItems[i]->m_quantity.get_mde(),
            normItems[i]->m_price.get_mde(),
            (*(normItems[i]->m_total)).get_mde());

        ticket.addCommodity(commodity);		// nothrow

        for (int k = 0; k < normItems[i]->m_discounts.size(); ++k )
		{
            vpmModifier_t modType(normItems[i]->m_discounts[k]->m_value.sign() < 0 ? VPM_MODIFIER_DISCOUNT : VPM_MODIFIER_MARKUP);

			libvpm::Modifier modifier(VPM_ITEM_REGULAR,		// nothrow
				modType,
                normItems[i]->m_discounts[k]->m_name.toStdString(),
                abs(normItems[i]->m_discounts[k]->m_value.get_mde()));

			ticket.addModifier(modifier);	// nothrow
		}		
	}

    // TODO: Налоги доделать потом
    /*for (int i = 0; i < __receipt.m_ReceiptTaxes.m_ItemTaxes.size(); ++i)
	{
		currency iPrice(*__receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_pItem->m_fTotal + *__receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_pItem->m_fDiscountIncrement);
		currency woTax = iPrice - __receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_fAmount;

		int tPercent = int(100. / woTax.get() * __receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_fAmount.get() + 0.5) * 1000;

		libvpm::Tax tax(VPM_TAX_VAT,
			VPM_TAXATION_STS,
			tPercent,
			__receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_fAmount.get() * Mult * Mult,
			true);

		ticket.addTax(tax);		// nothrow
    }

    for (int i = 0; i < __receipt->m_items.size(); ++i)
    {
        libvpm::Tax tax(VPM_TAX_VAT,
            VPM_TAXATION_STS,
            0,
            0,
            true);

        ticket.addTax(tax);		// nothrow
    }*/
	
    for (int i = 0; i < __receipt->m_payment_cash.size(); ++i)
    {
        vpmPayment_t pType = VPM_PAYMENT_CASH;

        libvpm::Payment payment(VPM_ITEM_REGULAR,
            pType,
            __receipt->m_payment_cash.at(i)->m_amount_with_change.get_mde());

        try
        {
            ticket.addPayment(payment);
        }
        catch(const libvpm::VpmTicketArgumentError& argument_e)
        {
            DEBUG_CHECKPOINT(logger::t_warning, QString("Argument error: %1").arg(argument_e.what()));
        }
        catch(const libvpm::VpmTicketNegativeMoneyError& money_e)
        {
            DEBUG_CHECKPOINT(logger::t_warning, QString("Money error: %1").arg(money_e.what()));
        }
    }

    for (int i = 0; i < __receipt->m_payment_cashless.size(); ++i)
    {
        vpmPayment_t pType = VPM_PAYMENT_CARD;
        libvpm::Payment payment(VPM_ITEM_REGULAR,
            pType,
            __receipt->m_payment_cashless.at(i)->m_amount.get_mde());

        try
        {
            ticket.addPayment(payment);
        }
        catch(const libvpm::VpmTicketArgumentError& argument_e)
        {
            DEBUG_CHECKPOINT(logger::t_warning, QString("Argument error: %1").arg(argument_e.what()));
        }
        catch(const libvpm::VpmTicketNegativeMoneyError& money_e)
        {
            DEBUG_CHECKPOINT(logger::t_warning, QString("Money error: %1").arg(money_e.what()));
        }
    }

	try
	{
        ticket.close(__receipt->getTotal().get_mde());
	}
	catch (const libvpm::VpmTicketError& e)
	{
		DEBUG_CHECKPOINT(logger::t_warning, QString("Cannot create ticket, %1").arg(e.what()));
        throw(std::runtime_error(e.what()));
	}

	try
	{
		m_instance->pingServer();
	}
	catch (const libvpm::VpmException& e)
	{
		DEBUG_CHECKPOINT(logger::t_info, QString("Cannot ping libvpm server, %1").arg(e.what()));
	}

	try
	{
		// Отправляем чек, получаем ответ
		libvpm::TicketResponse response = m_instance->processTicket(ticket);
	
		// Получаем фискальную надпечатку
		std::string res = response.fiscalText();
		__fiscalText = QString::fromStdString(res);
	}
	catch (const libvpm::VpmException& e)
	{
		DEBUG_CHECKPOINT(logger::t_warning, QString("Cannot process ticket, %1").arg(e.what()));
        throw(std::runtime_error(e.what()));
	}
}

void CGnivcSender::SendZReport(const SSCO::ShiftCloseV1Ptr __shift)
{
    if (m_instance == NULL)
    {
        throw(std::runtime_error("Библиотека libvpm не запущена. Данные отправляться не будут."));
    }

    const QDateTime epoch(QDate(1970,1,1), QTime(0,0,0));
    const quint64 secs = epoch.secsTo(__shift->m_date);

    LOG_MESSAGE(logger::t_info, "main",
                tr("ZReport time: DateTime(%1) Secs(%2)")
                .arg(__shift->m_date.toString("ddMMyyyy hhmmss"))
                .arg(secs));

	if (m_instance)
    {
		libvpm::ReportResponse zResponse = m_instance->processCloseShift(secs);
    }
}

void CGnivcSender::SendMoneyOperation(const SSCO::MoneyOperationV1Ptr __moHeader)
{
    if (m_instance == NULL)
    {
        throw(std::runtime_error("Библиотека libvpm не запущена. Данные отправляться не будут."));
    }

    const QDateTime epoch(QDate(1970,1,1), QTime(0,0,0));
    const quint64 secs = epoch.secsTo(__moHeader->m_date);

	vpmMoneyPlacement_t tType = VPM_MONEY_PLACEMENT_DEPOSIT;

    if (__moHeader->m_type == SSCO::MoneyOperationV1::PAID_OUT)
    {
		tType = VPM_MONEY_PLACEMENT_WITHDRAWAL;
    }

	try
	{
		libvpm::MoneyPlacementResponse response = m_instance->processMoneyPlacement(secs,
			tType,
            __moHeader->m_amount.get_mde());
	}
	catch (const libvpm::VpmException& e)
	{
		DEBUG_CHECKPOINT(logger::t_warning, QString("Cannot process money operation, %1").arg(e.what()));
    }
}

unsigned int CGnivcSender::CheckInstance()
{
    if(m_instance == NULL)
    {
        throw(std::runtime_error("Библиотека libvpm не запущена. Данные отправляться не будут."));
    }

    return static_cast<unsigned int>(m_instance->mode());
}

void CGnivcSender::SendXReport()
{
    if (m_instance == NULL)
    {
        throw(std::runtime_error("Библиотека libvpm не запущена. Данные отправляться не будут."));
    }

    const QDateTime epoch(QDate(1970,1,1), QTime(0,0,0));
    const quint64 secs = epoch.secsTo(QDateTime::currentDateTime());

	libvpm::ReportResponse xResponse = m_instance->processGetReport(VPM_REPORT_X, secs);
}

void CGnivcSender::procEvent(const int pause)
{
    QEventLoop tELoop;
    QTimer::singleShot(pause, &tELoop, SLOT(quit()));
    tELoop.exec();
}
