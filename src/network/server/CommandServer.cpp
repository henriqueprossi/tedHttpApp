#include <QCoreApplication>
#include <QJsonDocument>
#include <QTcpServer>
#include <QTcpSocket>
#include "CommandServer.h"
#include "Log.h"

CommandServer::CommandServer(QObject *parent)
    : QObject(parent) {

    Log::instance().log("CommandServer", "constructor");

    serverInit();
}

CommandServer::~CommandServer() {

    Log::instance().log("CommandServer", "destructor");
}

void CommandServer::serverInit() {

    m_tcpServer = new QTcpServer(this);

    connect(m_tcpServer, &QTcpServer::newConnection, this, &CommandServer::handleNewConnection);

    QString message;
    if (!m_tcpServer->listen(QHostAddress::Any, TCP_SERVER_PORT)) {
        message = "Server failed to listen on port " + QString::number(TCP_SERVER_PORT);
        Log::instance().log(
            "CommandServer",
            QCoreApplication::translate("server for commands", qUtf8Printable(message)));
        return;
    }

    message = "Server is listening on port " + QString::number(TCP_SERVER_PORT);
    Log::instance().log(
        "CommandServer",
        QCoreApplication::translate("server for commands", qUtf8Printable(message)));
}

QJsonObject CommandServer::processCommand(const QString tedIp, const QJsonObject &commandJson) {

    qint8 source = commandJson.value("source").toInt();
    QString text = commandJson.value("text").toString();

    Log::instance().log(
        "CommandServer",
        "Received from TED - source: " + QString::number(source) + ", text: " + text);

    emit textFromTed(tedIp, source, text);

    // Empty reply.
    QJsonObject replyBody;

    return replyBody;
}

void CommandServer::handleNewConnection() {

    QTcpSocket *socket = m_tcpServer->nextPendingConnection();

    if (!socket->waitForReadyRead(5000)) {
        Log::instance().log("CommandServer", "no data from TED!");
        return;
    }

    qint64 availableBytes = socket->bytesAvailable();

    Log::instance().log("CommandServer", "got " + QString::number(availableBytes) + " bytes!");

    QByteArray commandArray = socket->readAll();

    const QJsonObject commandJson = QJsonDocument::fromJson(commandArray).object();

    Log::instance().log("CommandServer", "command: " + commandArray);

    QHostAddress remoteAddress = socket->peerAddress();

    bool ipOk = false;
    QHostAddress tedIpV4(remoteAddress.toIPv4Address(&ipOk));
    QString tedIpStr;
    if (ipOk) {
        tedIpStr = tedIpV4.toString();
    }

    QJsonObject replyBody = processCommand(tedIpStr, commandJson);

    const QJsonDocument replyDoc(replyBody);
    QByteArray replyArray = replyDoc.toJson();
    socket->write(replyArray);
    socket->flush();
    socket->waitForBytesWritten(TCP_SEND_TIMEOUT_MS);
    //socket->close();
}
