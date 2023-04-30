#include <QCoreApplication>
#include <QJsonDocument>
#include "CommandServer.h"
#include "Log.h"

CommandServer::CommandServer(QObject *parent)
    : QObject(parent) {

    Log::instance().log("CommandServer", "constructor");

    httpServerInit();
}

CommandServer::~CommandServer() {

    Log::instance().log("CommandServer", "destructor");
}

void CommandServer::httpServerInit() {

    m_httpServer.route("/", QHttpServerRequest::Method::Post, [&](const QHttpServerRequest &request) {
        const QJsonObject commandJson = QJsonDocument::fromJson(request.body()).object();
        bool ipOk = false;
        QHostAddress tedIpV4(request.remoteAddress().toIPv4Address(&ipOk));
        QString tedIpStr;
        if (ipOk) {
            tedIpStr = tedIpV4.toString();
        }
        QJsonObject replyBody = processCommand(tedIpStr, commandJson);
        return QHttpServerResponse(replyBody, QHttpServerResponder::StatusCode::Created);
    });

    m_httpServer.afterRequest([](QHttpServerResponse &&resp) {        
        resp.setHeader("Server", "TED http server");
        return std::move(resp);
    });

    const auto port = m_httpServer.listen(QHostAddress::Any, HTTP_SERVER_PORT);
    if (!port) {
        QString message = "Server failed to listen on port " + QString::number(HTTP_SERVER_PORT);
        Log::instance().log(
            "CommandServer",
            QCoreApplication::translate("http server for commands", qUtf8Printable(message)));
        return;
    }
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
