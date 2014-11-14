#include "fnsserver.h"

#include "libssco/sax_serializer_base.h"

// Qt
#include <QTcpSocket>

FnsServer::FnsServer(QObject *parent) :
    QObject(parent)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        qDebug() << tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(procReceipt()));
}

bool FnsServer::parseReceipt(SSCO::ReceiptPtr __receipt)
{

}

void FnsServer::procReceipt()
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    QByteArray data;
    QByteArray close_request_tag = QString("</%1>").arg("SCO_UKM_PROTOCOL_RESPONSE").toUtf8();
    while (true)
    {
        QByteArray single_read;
        try
        {
            single_read.append(clientConnection->read(300));
        }
        catch (std::runtime_error& e)
        {
            qDebug() << QString("SCO - UKM Socket error: %1").arg(e.what());
            // TODO: return;
        }

        data += single_read;
        if (data.contains(close_request_tag) )
        {
            break;
        }
    }

    QXmlStreamReader reader (data);
    reader.readNext();
    reader.readNext();

    SSCO::ReceiptPtr receipt (new SSCO::Receipt());
    receipt->read(reader);

    clientConnection->disconnectFromHost();
}
