#ifndef FNSSERVER_H
#define FNSSERVER_H

#include <QObject>
#include <QTcpServer>

// Main
#include "libssco/receipt_v1.h"
#include "libssco/money_operation.h"
#include "libssco/shift_close.h"
#include "libssco/shift_open.h"
#include "libssco/fnsterminalprotocol.h"
#include "gnivc.h"
#include "liblogger/cdebug.h"

class FnsServer : public QObject
{
    Q_OBJECT
public:
    explicit FnsServer(const QString &__iniFile, QObject *parent = 0);
    ~FnsServer();

private:
    QTcpServer *tcpServer;
    CGnivcSender sender;

    CFNSTerminalResponsePtr execReceipt(const QString &__data, CFNSTerminalResponsePtr __responce);
    CFNSTerminalResponsePtr execZReport(const QString &__data, CFNSTerminalResponsePtr __responce);
    CFNSTerminalResponsePtr execXReport(CFNSTerminalResponsePtr __responce);
    CFNSTerminalResponsePtr execMoneyOperation(const QString &__data, CFNSTerminalResponsePtr __responce);

    void sendAnswer(QTcpSocket &socket, CFNSTerminalResponsePtr __answer);

signals:

public slots:
    void newDataToSend();

};

#endif // FNSSERVER_H
