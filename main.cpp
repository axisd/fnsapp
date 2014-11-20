#include <QCoreApplication>
#include <QTextCodec>

// main
#include "fnsserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    c_log().set_format(QString("'%1/logs-fnsservice'/yyyy/MM/yyyy-MM-dd'.log'").arg(
    #ifdef __linux__
                "/var/log/fnsservice"
    #else
                QCoreApplication::applicationDirPath()
    #endif //__linux__
                ), "HH:mm:ss");

    LOG_MESSAGE(logger::t_info, "main",
                QString("Start fnsapp"));

    QString listen_addr("127.0.0.1");

    QStringList args = a.arguments();
    for (int i = 1; i < args.size(); i++)
    {
        if (args[i] == "--listen")
        {
            if (i+1 < args.size())
            {
                listen_addr = args[i+1];
                i++;
            }
        }
    }

    LOG_MESSAGE(logger::t_info, "main",
                QString("Start fns app"));

    int ret(0);
    try
    {
        FnsServer server("vpm.ini", listen_addr, &a);
        ret = a.exec();
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main", e.what());
    }
    catch(...)
    {
        LOG_MESSAGE(logger::t_fatal, "main", "Non std exeption");
    }

    LOG_MESSAGE(logger::t_info, "main",
                QString("Exit fnsapp: %1").arg(ret));

    return ret;
}
