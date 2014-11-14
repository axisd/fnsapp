#ifndef FNSSERVER_H
#define FNSSERVER_H

#include <QObject>
#include <QTcpServer>

#include "libssco/receipt.h"

class FnsServer : public QObject
{
    Q_OBJECT
public:
    explicit FnsServer(QObject *parent = 0);

private:
    QTcpServer *tcpServer;

    bool parseReceipt(SSCO::ReceiptPtr __receipt);

signals:

public slots:
    void procReceipt();

};

#endif // FNSSERVER_H
