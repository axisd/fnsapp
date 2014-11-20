#ifndef FNSSERVER_H
#define FNSSERVER_H

#include <QObject>
#include <QTcpServer>

// Main
#include "libssco/receipt_v1.h"
#include "libssco/money_operation.h"
#include "libssco/shift_close_v1.h"
#include "libssco/shift_open.h"
#include "libssco/fns_protocol.h"
#include "gnivc.h"
#include "liblogger/cdebug.h"

class FnsServer : public QObject
{
    Q_OBJECT
public:
    explicit FnsServer(const QString &__iniFile, const QString &__listenAddr, QObject *parent = 0);
    ~FnsServer();

private:
    QTcpServer *tcpServer;
    CGnivcSender gnivc_sender;

    CFNSProtocolResponse execReceipt(const QString &__data);
    CFNSProtocolResponse execZReport(const QString &__data);
    CFNSProtocolResponse execXReport();
    CFNSProtocolResponse execMoneyOperation(const QString &__data);

    void sendAnswer(QTcpSocket &socket, CFNSProtocolResponse __answer);
    void procEvent(const int pause);

signals:

public slots:
    void newDataToSend();
    void readyRead();

};

#endif // FNSSERVER_H
