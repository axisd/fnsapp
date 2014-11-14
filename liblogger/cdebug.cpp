#include "cdebug.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QMutexLocker>

// BOOST
#include <boost/version.hpp>
#include <boost/format/exceptions.hpp>

using namespace std;

static QMutex mutex;

logger& c_log()
{
	static logger* r_log = NULL;
	if (r_log == NULL)
	{
		QMutexLocker locker(&mutex);
		if (r_log == NULL)
		{
			//Sequence of characters that are enclosed in single quotes will not be used as an expression.
			r_log = new logger(QString("'%1/logs'/yyyy/MM/yyyy-MM-dd'.log'").arg(QCoreApplication::applicationDirPath()), "HH:mm:ss");
			r_log->fill_entries(
#ifndef NDEBUG
				"qt;"
#endif
				"debug;sound;perfomance;DML;hw;diag;main;Global;ERROR;"
				"PERSCARD_STD2;DISCOUNT_ADDER;CCashLineCont;ServiceMain;"
				"StateMonitor;InvoicePrint;CLEAN;NtlpTrnManager;web_parse_mask;"
				"CReceiptPayment_NotWanted;impex;SBRF;cashier_shift;TAX;eej;context;"
				"PROTOCOL;LOGIN;AsynchronousMachine;NTLP;CKeylockIBM;CPinpadSC5000;CPinpadSC5000Bridge;"
				"CCOMPortStandard(1);CCOMPortStandard(2);CCOMPortStandard(3);"
				"CCOMPortStandard(4);CCOMPortStandard(5);CCOMPortStandard(6);"
				"CCOMPortStandard(7);CCOMPortStandard(8);CashierDisplayBMP;"
				"CSuspendedReceiptPrint;CAuthBonus;AuthCompass1;AuthCompass2;"
				"CAuthHelper1;CAuthHelper2;SPaymUCS;AuthUCS;"
				"AuthUCS_Commiter;CAuthVUZ;SPaymCyberCassa;SPaymRapida;"
				"CCash_loginner;CChannel_closer;"
				"GetDefaultConfigHandler;RegisterCashHandler;"
				"CAuthInternalSMBuffer;CAuthBonus;AuthRrcByFile;AuthRrcByPinpad;"
				"AuthRrcBySocket;ChangeDomainsModeHandler;SubmitDomainsHandler;"
				"JournalControlHandle;"
				"ServerLoginHandler;ServerChannelMaster;"
				"CGetReceiptFromOtherPosHandler;CInvoicePrintHandler;CServerPrintHandler;CCashUnblockHandler;CWebUKMRequest;"
				"CCashDocManager;Scheduler;"
				"BroadcastTrap;DiscountAdder;CAuthInternal_Commiter;CAnalyze_Commiter;trm_in_clients;"
				"CCashDocManager;CCertificateNumberManager;InvoicePrint;"
				"InvoiceNumberProcessor;Update monitor;"
				"ScalesHandler;Scale;CCore;Cash;OBSOLETE;WebNotify;converter;replication;prereplication;"
				"Connector to server;AuthBPC;luaukm;CityNet;RestMenu;HandleConverter;DiscountAddersTask;AuthGPB;OrderReplicator;TableInfoReplicator;"
				"CListener;PriceChecker;CPChServLoginHandler;PrintOrderPricetag;PSBank;"
				"MobileTerminal;SCOProtocol;posload;DatabaseCutDown;"
				"UpdatesDownloader;"
				"CBottomExecutor;Remain;UpdatesDownloadMonitor;CAuthRusselhoz;CAuthRucard;CAuthRucardTransportFileHandler;"
				"SoftCase;ReserveReceiptCommiter;PresentMailAuth;"
				"SatelliteServiceImport;SatelliteServiceOnlineCommiter;SatelliteServiceOfflineCommiter;"
				"COperDayShiftCommiter;CClientCardReplacer;CClientCreater;AuthEPOS;COperDay;GazpromBankAuth;"
				"PrivateBank;PrivateBankDebug;EXPORT_XML;mngsvc;mnginfra;licsvc;"
				"CAccountOfflineHandler;CAccountOnlineHandler;internal_accounting;"
				"CLoymaxOfflineCommiter;"
			);
		}
	}
	return *r_log;
}

void add_log_options_description(boost::program_options::options_description& __desc)
{
	__desc.add_options()
		("log-add", boost::program_options::value<std::string>(), "add logger item")
		("log-remove", boost::program_options::value<std::string>(), "remove logger item");
}

void process_log_options_description(const boost::program_options::variables_map& __vm)
{
	if (__vm.count("log-remove"))
	{
		c_log().remove_entries(QString::fromStdString(__vm["log-remove"].as<std::string>()));
	}

	if (__vm.count("log-add"))
	{
		c_log().add_entries(QString::fromStdString(__vm["log-add"].as<std::string>()));
	}
}

QString __source_point__(const QString& __file, int __line, const QString& __function)
{
	return QString("%1(%2) %3").arg(__file).arg(__line).arg( __function);
};

namespace boost{
	void BOOST_TT_DECL assertion_failed(char const * expr, char const * function, char const * file, long line)
	{
		throw runtime_error(QString("%1, expr: %2 ").arg(__source_point__(file, line, function), expr).toStdString());
	}
};
