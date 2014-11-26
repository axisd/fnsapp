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

    c_log().set_format(QString("'%1'/yyyy-MM-dd'.log'").arg(
    #ifdef __linux__
                "/var/log/fnsproxy"
    #else
                QCoreApplication::applicationDirPath()
    #endif //__linux__
                ), "HH:mm:ss");

    LOG_MESSAGE(logger::t_info, "main",
                QString("Start fnsapp"));

    QString listen_addr("127.0.0.1");
    QString ini_file("vpm.ini");

    const QStringList args = a.arguments();
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
        else if (args[i] == "--ini")
        {
            if (i+1 < args.size())
            {
                ini_file = args[i+1];
                i++;
            }
        }
    }

    LOG_MESSAGE(logger::t_info, "main",
                QString("Start fns app"));

    int ret(0);
    try
    {
        FnsServer server(ini_file, listen_addr, &a);

        LOG_MESSAGE(logger::t_info, "main", "Initialization vpm instance...");
        server.initVpm();
        LOG_MESSAGE(logger::t_info, "main", "Initialization vpm instance... SUCCESS");

        LOG_MESSAGE(logger::t_info, "main", "Starting listening service...");
        server.startServer();
        LOG_MESSAGE(logger::t_info, "main", "Starting listening service... SUCCESS");

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
