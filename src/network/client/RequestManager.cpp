#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "RequestManager.h"
#include "Log.h"

RequestManager::RequestManager(QObject *parent)
    : QObject(parent) {

    Log::instance().log("RequestManager", "constructor");
}

RequestManager::~RequestManager() {

    Log::instance().log("RequestManager", "destructor");
}

RequestManager::ResultStatus RequestManager::send(TedInfo &tedInfo, QString &requestBody, QString &replyBody) {

    QNetworkAccessManager nam;
    QEventLoop eventLoop;
    QTimer timeoutTimer;
    bool timeout = false;
    RequestManager::ResultStatus sendResult = RequestManager::ResultStatus::RES_FAIL;

    QNetworkRequest request;
    QString url = "http://" + tedInfo.m_ip + ":" + QString::number(tedInfo.m_port);
    request.setUrl(QUrl(url));
    request.setRawHeader("User-Agent", "tedHttpApp");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(&nam, &QNetworkAccessManager::finished,
            &eventLoop, &QEventLoop::quit);

    connect(&timeoutTimer, &QTimer::timeout, this, [&]() {
        timeout = true;
        eventLoop.quit();
    });

    timeoutTimer.setInterval(REQUEST_TIMEOUT_MS);
    timeoutTimer.setSingleShot(true);
    timeoutTimer.start();

    QByteArray bodyArray = requestBody.toUtf8();

    QNetworkReply *reply = nam.post(request, bodyArray);

    eventLoop.exec();

    //connect(reply, &QIODevice::readyRead, this, &Request::replyFinished);
    //connect(reply, &QNetworkReply::errorOccurred, this, &Request::replyError);

    QNetworkReply::NetworkError requestResult = reply->error() ;

    if (timeout || (requestResult != QNetworkReply::NoError)) {

        Log::instance().log("Request", "Reply error! error: " + QString::number(requestResult) + ", timeout: " + QString::number(timeout));

    } else {

        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        replyBody = QString(reply->readAll());

        Log::instance().log("Request", "Reply success! status code: " + QString::number(code) + ", body: " + replyBody);

        //emit replyReceived(replyBody);

        sendResult = RequestManager::ResultStatus::RES_SUCCESS;
    }

    delete reply;

    return sendResult;
}

void RequestManager::sendTextToTed(QString ip, quint16 port, QString text) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_SEND_TEXT_TO_TED);
    QJsonObject argsObj;
    argsObj.insert("text", text);
    bodyObj.insert("args", argsObj);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::clearDisplay(QString ip, quint16 port) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_CLEAR_DISPLAY);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::beepInit(QString ip, quint16 port) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_BEEP_INIT);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::beeps(QString ip, quint16 port, quint8 qtty) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_BEEPS);
    QJsonObject argsObj;
    argsObj.insert("qtty", qtty);
    bodyObj.insert("args", argsObj);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::sendToCOM1(QString ip, quint16 port, QString text) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_SEND_TO_COM1);
    QJsonObject argsObj;
    argsObj.insert("text", text);
    bodyObj.insert("args", argsObj);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::sendToCOM2(QString ip, quint16 port, QString text) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_SEND_TO_COM2);
    QJsonObject argsObj;
    argsObj.insert("text", text);
    bodyObj.insert("args", argsObj);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::readDigitalInput(QString ip, quint16 port) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_READ_DIGITAL_INPUT);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {

        emit replyReceived(ip, replyBody);

        QJsonObject jsonObj = QJsonDocument::fromJson(replyBody.toLatin1()).object();
        quint8 value = 0;
        if (jsonObj.contains("value")) {
            value = (quint8) jsonObj.value("value").toInt();
        }

        emit replyReceivedReadDigitalInput(value);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::turnOnDigitalInput(QString ip, quint16 port) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_TURN_ON_DIGITAL_INPUT);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::turnOffDigitalInput(QString ip, quint16 port) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_TURN_OFF_DIGITAL_INPUT);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}

void RequestManager::clearShortcutMenu(QString ip, quint16 port) {

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_CLEAR_SHORTCUT_MENU);

    QJsonDocument bodyDoc(bodyObj);
    QString requestBody = bodyDoc.toJson(QJsonDocument::Compact);

    TedInfo tedInfo = { .m_ip = ip, .m_port = port };
    QString replyBody;

    RequestManager::ResultStatus result = send(tedInfo, requestBody, replyBody);

    if (result == RequestManager::ResultStatus::RES_SUCCESS) {
        emit replyReceived(ip, replyBody);
    } else {
        emit replyTimeout(ip);
    }
}
