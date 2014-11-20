#include "fnsserver.h"

// Main
#include "libssco/sax_serializer_base.h"
#include "libssco/fns_task_description.h"

// Qt
#include <QTcpSocket>
#include <QEventLoop>
#include <QTimer>

#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(__linux__)
#include <unistd.h>
#endif

void sleep_ms(unsigned int ms)
{
#ifdef __linux__
    usleep(ms * 1000);
#endif
#ifdef _WIN32
    Sleep(ms);
#endif
}

int g_port = 6661;

FnsServer::FnsServer(const QString &__iniFile, const QString &__listenAddr, QObject *parent) :
    QObject(parent)
{
    tcpServer = new QTcpServer(this);

    if (!tcpServer->listen(QHostAddress(__listenAddr), g_port)) {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newDataToSend()), Qt::DirectConnection);

    if(!gnivc_sender.initLibrary(__iniFile))
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

CFNSProtocolResponse FnsServer::execReceipt(const QString &__data)
{    
    LOG_MESSAGE(logger::t_info, "main", QString("execReceipt. Data: %1").arg(__data));

    QString fiscalText;
    SSCO::ReceiptV1Ptr receipt (new SSCO::ReceiptV1);

    try
    {
        QXmlStreamReader reader	(__data);
        reader.readNext();
        reader.readNext();

        receipt->read(reader);
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при разборе чека: %1").arg(e.what()));
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, "");
        return resp;
    }


    LOG_MESSAGE(logger::t_info, "main",
                tr("Чек получен"));

    try
    {
        gnivc_sender.SendReceipt(receipt, fiscalText);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_SUCCESS, fiscalText);
        LOG_MESSAGE(logger::t_info, "main",
                    tr("Чек успешно обработан. fiscalText: %1").arg(fiscalText));
        return resp;
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при отправке чека: %1.")
                    .arg(e.what()));
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, "");
        return resp;
    }
}

CFNSProtocolResponse FnsServer::execZReport(const QString &__data)
{
    QXmlStreamReader reader(__data);
    reader.readNext();
    reader.readNext();

    SSCO::ShiftCloseV1Ptr sc (new SSCO::ShiftCloseV1);
    sc->read(reader);

    try
    {
        gnivc_sender.SendZReport(sc);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_SUCCESS, "");
        return resp;
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при отправке Z-отчета: %1.")
                    .arg(e.what()));
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, "");
        return resp;
    }
}

CFNSProtocolResponse FnsServer::execXReport()
{
    try
    {
        gnivc_sender.SendXReport();
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_SUCCESS, "");
        return resp;
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при отправке X-отчета: %1.")
                    .arg(e.what()));
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, "");
        return resp;
    }
}

CFNSProtocolResponse FnsServer::execMoneyOperation(const QString &__data)
{
    QXmlStreamReader reader(__data);
    reader.readNext();
    reader.readNext();

    SSCO::MoneyOperationPtr mo (new SSCO::MoneyOperation);
    mo->read(reader);

    try
    {
        gnivc_sender.SendMoneyOperation(mo);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_SUCCESS, "");
        return resp;
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при отправке денежной операции: %1.")
                    .arg(e.what()));
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, "");
        return resp;
    }
}

void FnsServer::sendAnswer(QTcpSocket &socket, CFNSProtocolResponse __answer)
{    
    QByteArray data;
    QXmlStreamWriter data_writer (&data);
    __answer.write(data_writer);

    LOG_MESSAGE(logger::t_info, "main", QString("Send answer: %1").arg(QString(data)));

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
    LOG_MESSAGE(logger::t_info, "main", "New connection");
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void FnsServer::readyRead()
{
    LOG_MESSAGE(logger::t_info, "main", QString("Ready Read"));

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());

    uint number_of_retries = 3;

    QByteArray data;
    QByteArray close_request_tag = QString("</%1>").arg("FNS_REQUEST").toUtf8();
    while (socket->bytesAvailable() > 0)
    {
        QByteArray single_read;
        try
        {
            single_read.append(socket->readAll());
        }
        catch (std::runtime_error& e)
        {
            LOG_MESSAGE(logger::t_info, "main", QString("FNS - UKM Socket error: %1").arg(e.what()));
            // TODO: return;
            return;
        }

        LOG_MESSAGE(logger::t_info, "main", QString("Size: %2 Data: %1").arg(QString(single_read)).arg(single_read.size()));

        data += single_read;
        if (data.contains(close_request_tag) )
        {
            break;
        }
        --number_of_retries;
        if(number_of_retries <= 0)
        {
            LOG_MESSAGE(logger::t_info, "main", QString("Превышен интервал ожидания запроса"));
            break;
        }
        procEvent(200);
    }

    LOG_MESSAGE(logger::t_info, "main", QString("Size: %2 Data: %1").arg(QString(data)).arg(data.size()));

    QXmlStreamReader req_reader (QString::fromUtf8(data));
    CFNSProtocolRequest request;
    request.deserialize(req_reader);

    switch (request.getTaskEfts())
    {
    case FNS_SEND_RECEIPT:
    {
        sendAnswer(*socket, execReceipt(request.getData()));
        break;
    }
    case FNS_SEND_MONEY_OPERATION:
    {
        sendAnswer(*socket, execMoneyOperation(request.getData()));
        break;
    }
    case FNS_SEND_X_REPORT:
    {
        sendAnswer(*socket, execXReport());
        break;
    }
    case FNS_SEND_Z_REPORT:
    {
        sendAnswer(*socket, execZReport(request.getData()));
        break;
    }
    default:
        throw std::runtime_error("Unknown Task Type");
    }

    socket->disconnectFromHost();
}

void FnsServer::procEvent(const int pause)
{
    QEventLoop tELoop;
    QTimer::singleShot(pause, &tELoop, SLOT(quit()));
    tELoop.exec();
}
