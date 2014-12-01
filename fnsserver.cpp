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
    m_iniFileName(__iniFile),
    m_listenIP(__listenAddr),
    m_tcpServer(NULL),
    QObject(parent)
{

}

void FnsServer::initVpm()
{
    if(!gnivc_sender.initLibrary(m_iniFileName))
    {
        LOG_MESSAGE(logger::t_fatal, "main", "Initialization vpm instance... FAIL");
        throw(std::runtime_error(QString("Unable to init fns library.").toStdString().c_str()));
    }
}

void FnsServer::startServer()
{
    if(!m_tcpServer)
    {
        m_tcpServer = new QTcpServer(this);

        if (!m_tcpServer->listen(QHostAddress(m_listenIP), g_port))
        {
            LOG_MESSAGE(logger::t_fatal, "main", "Starting listening service... FAIL");
            throw(std::runtime_error(QString("Unable to start the server: %1.").arg(m_tcpServer->errorString()).toStdString().c_str()));
        }

        connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newDataToSend()), Qt::DirectConnection);
    }
    else
    {
        throw(std::runtime_error("Server is already created"));
    }
}

FnsServer::~FnsServer()
{
    if(m_tcpServer)
    {
        if(m_tcpServer->isListening())
        {
            m_tcpServer->close();
        }

        delete m_tcpServer;
        m_tcpServer = NULL;
    }

    try
    {
        gnivc_sender.StopInstance();
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main", QString("Error when stopping fns instance: %1").arg(e.what()));
    }
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
        const QString msg(tr("Ошибка при разборе чека: %1").arg(e.what()));
        LOG_MESSAGE(logger::t_fatal, "main", msg);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
        return resp;
    }


    LOG_MESSAGE(logger::t_info, "main",
                tr("Чек получен"));

    try
    {
        gnivc_sender.SendReceipt(receipt, fiscalText);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_SUCCESS, fiscalText);
        LOG_MESSAGE(logger::t_info, "main",
                    tr("Чек успешно обработан. fiscalText:\n%1").arg(fiscalText));
        return resp;
    }
    catch(std::exception &e)
    {
        const QString msg(tr("Ошибка при отправке чека: %1.").arg(e.what()));
        LOG_MESSAGE(logger::t_fatal, "main", msg);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
        return resp;
    }
}

CFNSProtocolResponse FnsServer::execZReport(const QString &__data)
{
    SSCO::ShiftCloseV1Ptr sc (new SSCO::ShiftCloseV1);

    try
    {
        QXmlStreamReader reader(__data);
        reader.readNext();
        reader.readNext();

        sc->read(reader);
    }
    catch(std::exception &e)
    {
        const QString msg(tr("Ошибка при разборе Z-отчета: %1").arg(e.what()));
        LOG_MESSAGE(logger::t_fatal, "main", msg);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
        return resp;
    }

    try
    {
        gnivc_sender.SendZReport(sc);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_SUCCESS, "");
        return resp;
    }
    catch(std::exception &e)
    {
        const QString msg(tr("Ошибка при отправке Z-отчета: %1.").arg(e.what()));
        LOG_MESSAGE(logger::t_fatal, "main", msg);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
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
        const QString msg(tr("Ошибка при отправке X-отчета: %1.").arg(e.what()));
        LOG_MESSAGE(logger::t_fatal, "main", msg);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
        return resp;
    }
}

CFNSProtocolResponse FnsServer::execMoneyOperation(const QString &__data)
{
    SSCO::MoneyOperationV1Ptr mo (new SSCO::MoneyOperationV1);

    try
    {
        QXmlStreamReader reader(__data);
        reader.readNext();
        reader.readNext();

        mo->read(reader);
    }
    catch(std::exception &e)
    {
        const QString msg(tr("Ошибка при разборе денежной операции: %1").arg(e.what()));
        LOG_MESSAGE(logger::t_fatal, "main", msg);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
        return resp;
    }

    try
    {
        gnivc_sender.SendMoneyOperation(mo);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_SUCCESS, "");
        return resp;
    }
    catch(std::exception &e)
    {
        const QString msg(tr("Ошибка при отправке денежной операции: %1.").arg(e.what()));
        LOG_MESSAGE(logger::t_fatal, "main", msg);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
        return resp;
    }
}

CFNSProtocolResponse FnsServer::execPingFNS()
{
    try
    {
        unsigned int status = gnivc_sender.CheckInstance();
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_SUCCESS, QString("%1").arg(status));
        return resp;
    }
    catch(std::exception &e)
    {
        const QString msg(tr("Ошибка при отправке денежной операции: %1.").arg(e.what()));
        LOG_MESSAGE(logger::t_fatal, "main", msg);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
        return resp;
    }
}

void FnsServer::sendAnswer(QTcpSocket &socket, CFNSProtocolResponse __answer)
{    
    QByteArray data;

    try
    {
        QXmlStreamWriter data_writer (&data);
        __answer.write(data_writer);
    }
    catch(std::exception &e)
    {
        LOG_MESSAGE(logger::t_fatal, "main",
                    tr("Ошибка при формировании ответа: %1").arg(e.what()));
        return;
    }

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
        return;
    }
}

void FnsServer::newDataToSend()
{
    LOG_MESSAGE(logger::t_info, "main", "New connection");
    QTcpSocket *clientConnection = m_tcpServer->nextPendingConnection();
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
            socket->disconnectFromHost();
            return;
        }

        // LOG_MESSAGE(logger::t_info, "main", QString("Size: %2 Data: %1").arg(QString(single_read)).arg(single_read.size()));

        data += single_read;
        if (data.contains(close_request_tag) )
        {
            break;
        }

        --number_of_retries;
        if(number_of_retries <= 0)
        {
            LOG_MESSAGE(logger::t_info, "main", QString("Превышен интервал ожидания запроса"));
            socket->disconnectFromHost();
            return;
        }
        procEvent(200);
    }

    LOG_MESSAGE(logger::t_info, "main", QString("Size: %2 Data: %1").arg(QString(data)).arg(data.size()));

    CFNSProtocolRequest request;

    try
    {
        QXmlStreamReader req_reader (QString::fromUtf8(data));
        request.deserialize(req_reader);
    }
    catch(std::exception &e)
    {
        const QString msg = tr("Ошибка при разборе запроса: %1").arg(e.what());
        LOG_MESSAGE(logger::t_fatal, "main", msg);
        CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
        sendAnswer(*socket, resp);
        return;
    }

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
        case FNS_PING:
        {
            sendAnswer(*socket, execPingFNS());
            break;
        }
        default:
        {
            const QString msg = tr("Unknown Task Type: %1").arg(request.getTaskEfts());
            LOG_MESSAGE(logger::t_fatal, "main", msg);
            CFNSProtocolResponse resp(FNS_TASK_FINISHED_FAIL, msg);
            sendAnswer(*socket, resp);
        }
    }

    socket->disconnectFromHost();
}

void FnsServer::procEvent(const int pause)
{
    QEventLoop tELoop;
    QTimer::singleShot(pause, &tELoop, SLOT(quit()));
    tELoop.exec();
}
