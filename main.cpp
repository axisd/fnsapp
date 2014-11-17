#include <QCoreApplication>

// main
#include "fnsserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    c_log().set_format(QString("'%1/logs-fnsservice'/yyyy/MM/yyyy-MM-dd'.log'").arg(
    #ifdef __linux__
                "/var/log/fnsservice"
    #else
                QCoreApplication::applicationDirPath()
    #endif //__linux__
                ), "HH:mm:ss");

    return a.exec();
}
