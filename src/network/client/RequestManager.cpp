#include <QJsonObject>
#include <QJsonDocument>
#include "RequestManager.h"
#include "RequestSendTextToTed.h"
#include "Log.h"

RequestManager::RequestManager(QObject *parent)
    : QObject(parent) {

    Log::instance().log("RequestManager", "constructor");
}

RequestManager::~RequestManager() {

    Log::instance().log("RequestManager", "destructor");
}

void RequestManager::sendTextToTed(QString ip, quint16 port, QString text) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", 1);
    QJsonObject argsObj;
    argsObj.insert("text", text);
    bodyObj.insert("args", argsObj);

    QJsonDocument bodyDoc(bodyObj);
    QString body = bodyDoc.toJson(QJsonDocument::Compact);

    RequestSendTextToTed request(ip, port, body, this);

    QString replyBody;

    RequestResult::ResultStatus result = request.send(replyBody);

    if (result == RequestResult::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::clearDisplay(QString ip, quint16 port) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", 3);

    QJsonDocument bodyDoc(bodyObj);
    QString body = bodyDoc.toJson(QJsonDocument::Compact);

    RequestSendTextToTed request(ip, port, body, this);

    QString replyBody;

    RequestResult::ResultStatus result = request.send(replyBody);

    if (result == RequestResult::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}
