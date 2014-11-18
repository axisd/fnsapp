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

    FnsServer server("vpm.ini", &a);

    int ret(0);
    try
    {
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

    return ret;
}
