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

    bool execReceipt(SSCO::ReceiptPtr __receipt, QString &__fiscalText);

signals:

public slots:
    void newReceipt();

};

#endif // FNSSERVER_H
