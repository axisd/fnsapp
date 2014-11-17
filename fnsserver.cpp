#include "fnsserver.h"

// Main
#include "libssco/sax_serializer_base.h"
#include "libssco/fns_task_description.h"

// Qt
#include <QTcpSocket>

FnsServer::FnsServer(const QString &__iniFile, QObject *parent) :
    QObject(parent)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newDataToSend()));

    if(!sender.initLibrary(__iniFile))
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Unable to init fns library."));
        return;
    }
}

FnsServer::~FnsServer()
{
    if(tcpServer->isListening())
    {
        tcpServer->close();
    }

    delete tcpServer;
}

CFNSTerminalResponsePtr FnsServer::execReceipt(const QString &__data, CFNSTerminalResponsePtr __responce)
{
    CFNSTerminalResponsePtr resp;
    QString fiscalText;

    QXmlStreamReader reader	(__data);
    reader.readNext();
    reader.readNext();

    SSCO::ReceiptV1Ptr receipt (new SSCO::ReceiptV1);
    receipt->read(reader);

    try
    {
        sender.SendReceipt(receipt, fiscalText);
        resp = CFNSTerminalResponsePtr(new CFNSTerminalResponse(__responce->m_task_guid, FNS_TASK_FINISHED_SUCCESS, fiscalText));
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при отправке чека: %1.")
                    .arg(e.what()));
        resp = CFNSTerminalResponsePtr(new CFNSTerminalResponse(__responce->m_task_guid, FNS_TASK_FINISHED_FAIL, ""));
    }

    return resp;
}

CFNSTerminalResponsePtr
FnsServer::execZReport(const QString &__data, CFNSTerminalResponsePtr __responce)
{
    CFNSTerminalResponsePtr resp;

    QXmlStreamReader reader(__data);
    reader.readNext();
    reader.readNext();

    SSCO::ShiftClosePtr sc (new SSCO::ShiftClose);
    sc->read(reader);

    try
    {
        sender.SendZReport(sc);
        resp = CFNSTerminalResponsePtr(new CFNSTerminalResponse(__responce->m_task_guid, FNS_TASK_FINISHED_SUCCESS, ""));
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при отправке Z-отчета: %1.")
                    .arg(e.what()));
        resp = CFNSTerminalResponsePtr(new CFNSTerminalResponse(__responce->m_task_guid, FNS_TASK_FINISHED_FAIL, ""));
    }

    return resp;
}

CFNSTerminalResponsePtr
FnsServer::execXReport(CFNSTerminalResponsePtr __responce)
{
    CFNSTerminalResponsePtr resp;

    try
    {
        sender.SendXReport();
        resp = CFNSTerminalResponsePtr(new CFNSTerminalResponse(__responce->m_task_guid, FNS_TASK_FINISHED_SUCCESS, ""));
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при отправке X-отчета: %1.")
                    .arg(e.what()));
        resp = CFNSTerminalResponsePtr(new CFNSTerminalResponse(__responce->m_task_guid, FNS_TASK_FINISHED_FAIL, ""));
    }

    return resp;
}

CFNSTerminalResponsePtr
FnsServer::execMoneyOperation(const QString &__data, CFNSTerminalResponsePtr __responce)
{
    CFNSTerminalResponsePtr resp;

    QXmlStreamReader reader(__data);
    reader.readNext();
    reader.readNext();

    SSCO::MoneyOperationPtr mo (new SSCO::MoneyOperation);
    mo->read(reader);

    try
    {
        sender.SendMoneyOperation(mo);
        resp = CFNSTerminalResponsePtr(new CFNSTerminalResponse(__responce->m_task_guid, FNS_TASK_FINISHED_SUCCESS, ""));
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при отправке денежной операции: %1.")
                    .arg(e.what()));
        resp = CFNSTerminalResponsePtr(new CFNSTerminalResponse(__responce->m_task_guid, FNS_TASK_FINISHED_FAIL, ""));
    }

    return resp;
}

void FnsServer::sendAnswer(QTcpSocket &socket, CFNSTerminalResponsePtr __answer)
{
    QByteArray data;
    QXmlStreamWriter data_writer (&data);
    __answer->serialize(data_writer);

    try
    {
        socket.write(data);
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при отправке ответа: %1.")
                    .arg(e.what()));
    }
}

void FnsServer::newDataToSend()
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    QByteArray data;
    QByteArray close_request_tag = QString("</%1>").arg("SCO_UKM_PROTOCOL_REQUEST").toUtf8();
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

    QXmlStreamReader req_reader (QString::fromUtf8(data));
    CFNSTerminalRequest request;
    request.deserialize(req_reader);

    CFNSTerminalResponsePtr response(new CFNSTerminalResponse(request.getTaskGuid(), FNS_TASK_IN_PROGRESS, ""));

    switch (request.getTaskEfts())
    {
    case FNS_SEND_RECEIPT:
    {
        sendAnswer(*clientConnection, execReceipt(request.getData(), response));
        break;
    }
    case FNS_SEND_MONEY_OPERATION:
    {
        sendAnswer(*clientConnection, execMoneyOperation(request.getData(), response));
        break;
    }
    case FNS_SEND_X_REPORT:
    {
        sendAnswer(*clientConnection, execXReport(response));
        break;
    }
    case FNS_SEND_Z_REPORT:
    {
        sendAnswer(*clientConnection, execZReport(request.getData(), response));
        break;
    }
    default:
        throw std::runtime_error("Unknown Task Type");
    }

    clientConnection->disconnectFromHost();
}
