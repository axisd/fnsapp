#include "gnivc.h"

// System
#include <stdint.h>

// Main
#include "liblogger/cdebug.h"

// Qt
#include <QFile>

#ifdef __linux__
    struct termios new_settings;
    struct termios stored_settings;
#endif

const double Mult = 0.1;

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
//	settings.setToken(96355186);
//	settings.setTimeout(3000);
//	settings.setHost("k-server.test-gnivc.ru");
//	settings.setKkmId(10046);
//	settings.setPort(7777);
//	settings.setSsl(false);

    settings = libvpm::SettingsLoader::loadFromFile(__iniFile.toStdString(), true);

    bool ok(false);

	while (!ok)
	{
		m_instance = new libvpm::Instance(settings);
		m_instance->start();

        //sleep_ms(1000);

        if (m_instance->mode() != MODE_NORMAL || m_instance->mode() != MODE_RESCUE)
        {
			ok = true;
        }
	}

	ClearToken(__iniFile);

    return ok;
}

void CGnivcSender::StopInstance(void)
{
	if (m_instance != NULL)
    {
		m_instance->stop();
    }
}

CGnivcSender::CGnivcSender()
{}

CGnivcSender::~CGnivcSender()
{}

void CGnivcSender::SendReceipt(const SSCO::ReceiptV1Ptr __receipt, QString& __fiscalText)
{
	if (m_instance == NULL)
    {
		throw(std::runtime_error("Библиотека libvpm не запущена. Данные отправляться не будут."));
    }

//	if (m_instance->mode() != MODE_NORMAL && m_instance->mode() != MODE_RESCUE)
	//	throw(std::runtime_error(QString("Р РµР¶РёРј libvpm %1. Р”Р°РЅРЅС‹Рµ РѕС‚РїСЂР°РІР»РµРЅС‹ РЅРµ Р±СѓРґСѓС‚.").arg(m_instance->mode()).toStdString()));

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

    case SSCO::ModelReceiptV1::HEADER_STOCK_COUNT :
		break;

	default :
		DEBUG_ASSERT(false);
	};

    int secs = __receipt->m_open_datetime.toTime_t();

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
            normItems[i]->m_quantity.get() * Mult,
            normItems[i]->m_price.get() * Mult * 0.1,
            (*(normItems[i]->m_total)).get() * Mult * 0.1);

        for (int k = 0; k < normItems[i]->m_discounts.size(); ++k )
		{
            vpmModifier_t modType(normItems[i]->m_discounts[k]->m_value.sign() < 0 ? VPM_MODIFIER_DISCOUNT : VPM_MODIFIER_MARKUP);

			libvpm::Modifier modifier(VPM_ITEM_REGULAR,		// nothrow
				modType,
                normItems[i]->m_discounts[k]->m_name.toStdString(),
                abs(normItems[i]->m_discounts[k]->m_value.get()) * Mult * 0.1);

			ticket.addModifier(modifier);	// nothrow
		}

		ticket.addCommodity(commodity);		// nothrow
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
    }*/

    for (int i = 0; i < __receipt->m_items.size(); ++i)
    {
        libvpm::Tax tax(VPM_TAX_VAT,
            VPM_TAXATION_STS,
            0,
            0,
            true);

        ticket.addTax(tax);		// nothrow
    }
	
    if (__receipt->m_payment_cash)
    {
        vpmPayment_t pType = VPM_PAYMENT_CASH;

        libvpm::Payment payment(VPM_ITEM_REGULAR,
            pType,
            __receipt->m_payment_cash->m_amount_with_change.get() * Mult * 0.1);

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

    if (__receipt->m_payment_cashless)
    {
        vpmPayment_t pType = VPM_PAYMENT_CARD;
        libvpm::Payment payment(VPM_ITEM_REGULAR,
            pType,
            __receipt->m_payment_cashless->m_amount.get() * Mult * 0.1);

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
        ticket.close((__receipt->getTotal().get()) * Mult * 0.1);
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

void CGnivcSender::SendShiftClose(const SSCO::ShiftClosePtr __shift)
{
    int secs = __shift->m_date.toTime_t();

	if (m_instance)
    {
		libvpm::ReportResponse zResponse = m_instance->processCloseShift(secs);
    }
}

void CGnivcSender::MoneyInsert(const SSCO::MoneyOperationPtr __moHeader)
{
    int secs = __moHeader->m_date.toTime_t();

	vpmMoneyPlacement_t tType = VPM_MONEY_PLACEMENT_DEPOSIT;

    if (__moHeader->m_type == SSCO::MoneyOperation::PAID_OUT)
    {
		tType = VPM_MONEY_PLACEMENT_WITHDRAWAL;
    }

    long long am = __moHeader->m_amount.get() * Mult * Mult;

	try
	{
		libvpm::MoneyPlacementResponse response = m_instance->processMoneyPlacement(secs,
			tType,
			am);
	}
	catch (const libvpm::VpmException& e)
	{
		DEBUG_CHECKPOINT(logger::t_warning, QString("Cannot process money operation, %1").arg(e.what()));
	}
}

void CGnivcSender::SendXReport()
{
    int secs = QDateTime::currentDateTime().toTime_t();

	libvpm::ReportResponse xResponse = m_instance->processGetReport(VPM_REPORT_X, secs);
}
