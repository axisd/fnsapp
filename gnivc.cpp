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
//		settings.setToken(96355186);
	settings.setTimeout(3000);
	settings.setHost("k-server.test-gnivc.ru");
	settings.setKkmId(10046);
	settings.setPort(7777);
	settings.setSsl(false);
//		settings = libvpm::SettingsLoader::loadFromFile(__iniFile.toStdString(), true);

	bool ok(false), answered(false);

	while (!ok)
	{
		if (answered)
        {
			ok = true;
        }

		m_instance = new libvpm::Instance(settings);
		m_instance->start();

		sleep_ms(1000);

		if (m_instance->mode() == MODE_BLOCKED_INVALID_TOKEN && !answered)
		{
			m_instance->stop();
			delete (m_instance);
			m_instance = NULL;

			QString szInp;
			if (g_UserInterface->InputLineBox(QObject::tr("Неверный токен. Введите новый токен для кассы"), szInp))
			{
					settings.setToken(szInp.toUInt());
					answered = true;
			}
			else
			{
				g_UserInterface->MessageBox(QObject::tr("���������� libvpm �� ��������. ������ ������������ �� �����."), false);
				ok = true;
			}
		}
		else
        {
			ok = true;
        }
	}

	ClearToken(__iniFile);

	return true;
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

void CGnivcSender::SendReceipt(const SSCO::ReceiptPtr __receipt, QString& __fiscalText)
{
	if (m_instance == NULL)
    {
		throw(std::runtime_error("Библиотека libvpm не запущена. Данные отправляться не будут."));
    }

//	if (m_instance->mode() != MODE_NORMAL && m_instance->mode() != MODE_RESCUE)
	//	throw(std::runtime_error(QString("Р РµР¶РёРј libvpm %1. Р”Р°РЅРЅС‹Рµ РѕС‚РїСЂР°РІР»РµРЅС‹ РЅРµ Р±СѓРґСѓС‚.").arg(m_instance->mode()).toStdString()));

	vpmOperation_t op;
	switch (__receipt.m_ReceiptHeader->m_nType)
	{
	case HEADER_SALE :
	case HEADER_POP :
		op = VPM_OPERATION_SELL;
		break;

	case HEADER_RETURN :
	case HEADER_RETURN_BY_RECEIPT :
		op = VPM_OPERATION_SELL_RETURN;
		break;

	case HEADER_STOCK_COUNT :
		break;

	default :
		DEBUG_ASSERT(false);
	};

	boost::posix_time::ptime start(boost::gregorian::date(1970,1,1));
	int secs = (__receipt.m_ReceiptHeader->m_Date - start).total_seconds();

	libvpm::Ticket ticket(VPM_DOMAIN_TRADING, // nothrow
		op,
		secs,
		__receipt.CoreLoginUserId(),
		__receipt.CoreLoginUserName().toStdString());

	QVector<CCoreOutReceiptItem_ptr> normItems = __receipt.m_ReceiptItems.get();

	for (int i = 0; i < normItems.size(); ++i)
	{
		vpmStorno_t iType = VPM_ITEM_REGULAR;
		int iii = normItems[i]->m_nType;
		if (normItems[i]->m_nType == ITEM_VOID)
        {
			iType = VPM_ITEM_STORNO;
        }
		if (normItems[i]->m_nType == ITEM_CANCEL)
        {
			continue;
        }

		libvpm::Commodity commodity(iType,		// nothrow
			normItems[i]->m_szName.toStdString(),
			__receipt.m_ReceiptHeader->m_Stock.m_szName.toStdString(),
			normItems[i]->m_fQuantity.get() * Mult,
			normItems[i]->m_fPrice.get() * Mult * 0.1,
			(*(normItems[i]->m_fTotal)).get() * Mult * 0.1);

		for (int k = 0; k < normItems[i]->m_Discounts.size(); ++k )
		{
			vpmModifier_t modType(normItems[i]->m_Discounts[k]->m_fIncrement->sign() < 0 ? VPM_MODIFIER_DISCOUNT : VPM_MODIFIER_MARKUP);

			libvpm::Modifier modifier(VPM_ITEM_REGULAR,		// nothrow
				modType,
				normItems[i]->m_Discounts[k]->m_pReceiptDiscount->m_szName.toStdString(),
				abs(normItems[i]->m_Discounts[k]->m_fIncrement->get()) * Mult * 0.1);

			ticket.addModifier(modifier);	// nothrow
		}

		ticket.addCommodity(commodity);		// nothrow
	}

	QVector<CCoreOutReceiptItem_ptr> norm(__receipt.m_ReceiptItems.normal());
	
	for (int i = 0; i < __receipt.m_ReceiptTaxes.m_ItemTaxes.size(); ++i)
	{
		currency iPrice(*__receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_pItem->m_fTotal + *__receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_pItem->m_fDiscountIncrement);
		currency woTax = iPrice - __receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_fAmount;

		int tPercent = int(100. / woTax.get() * __receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_fAmount.get() + 0.5) * 1000;
		int iPerc = __receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_szPercent.toUInt() * 1000;

		libvpm::Tax tax(VPM_TAX_VAT,
			VPM_TAXATION_STS,
			tPercent,
			__receipt.m_ReceiptTaxes.m_ItemTaxes[i]->m_fAmount.get() * Mult * Mult,
			true);

		ticket.addTax(tax);		// nothrow
	}
	
	CCoreOutReceiptPaymentRecordset normPayments;

	__receipt.m_ReceiptPayments.LeaveNormal(normPayments);

	for (int j = 0; j < normPayments.size(); ++j)
	{
		vpmPayment_t pType = VPM_PAYMENT_CASH;

		if (!(normPayments[j]->GetEFTS() == efts::paym::LOCAL))
        {
			pType = VPM_PAYMENT_CARD;
        }

		libvpm::Payment payment(VPM_ITEM_REGULAR,
			pType,
			normPayments[j]->m_AmountWithChange.get() * Mult * 0.1);

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
		ticket.close((__receipt.m_fReceiptAmount->get() + __receipt.m_fReceiptDiscountAmount->get()) * Mult * 0.1);
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
	boost::posix_time::ptime start(boost::gregorian::date(1970,1,1));
	int secs = (__shift->m_Date - start).total_seconds();

	if (m_instance)
    {
		libvpm::ReportResponse zResponse = m_instance->processCloseShift(secs);
    }
}

void CGnivcSender::MoneyInsert(const SSCO::MoneyOperationPtr __moHeader)
{
	boost::posix_time::ptime start(boost::gregorian::date(1970,1,1));
	int secs = (__moHeader->m_Date - start).total_seconds();

	vpmMoneyPlacement_t tType = VPM_MONEY_PLACEMENT_DEPOSIT;

	if (__moHeader->m_nType == MONEY_OPERATION_EXTRACT)
    {
		tType = VPM_MONEY_PLACEMENT_WITHDRAWAL;
    }

	long long am = __moHeader->m_fAmount.get() * Mult * Mult;

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
	boost::posix_time::ptime pNow(boost::posix_time::second_clock::local_time());

	boost::posix_time::ptime start(boost::gregorian::date(1970,1,1));
	int secs = (pNow - start).total_seconds();

	libvpm::ReportResponse xResponse = m_instance->processGetReport(VPM_REPORT_X, secs);
}
