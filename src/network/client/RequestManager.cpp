#include <QJsonObject>
#include <QJsonDocument>
#include <QTcpSocket>
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

    Log::instance().log("RequestManager", "sending to " + tedInfo.m_ip + ":" + QString::number(tedInfo.m_port) +
                                              " -> " + requestBody);

    QTcpSocket socket;
    socket.connectToHost(tedInfo.m_ip, tedInfo.m_port);

    Log::instance().log("RequestManager", "waiting connection...");

    if (!socket.waitForConnected(REQUEST_TIMEOUT_MS)) {
        Log::instance().log("RequestManager", "not connected!");
        return RequestManager::ResultStatus::RES_FAIL;
    }

    Log::instance().log("RequestManager", "connected!");

    QByteArray bodyArray = requestBody.toUtf8();

    Log::instance().log("RequestManager", "writting...");

    qint64 writtenBytes = socket.write(bodyArray);
    if (writtenBytes != bodyArray.size()) {
        Log::instance().log("RequestManager", "write call error!");
        return RequestManager::ResultStatus::RES_FAIL;
    }

    if (!socket.waitForBytesWritten(REQUEST_TIMEOUT_MS)) {
        Log::instance().log("RequestManager", "wait write call error!");
        return RequestManager::ResultStatus::RES_FAIL;
    }

    Log::instance().log("RequestManager", "reading reply...");

    if (!socket.waitForReadyRead(REQUEST_TIMEOUT_MS)) {
        Log::instance().log("RequestManager", "no reply from TED!");
        return RequestManager::ResultStatus::RES_FAIL;
    }

    qint64 availableBytes = socket.bytesAvailable();

    Log::instance().log("RequestManager", "got " + QString::number(availableBytes) + " bytes!");

    QByteArray replyArray = socket.readAll();

    replyBody = QString::fromUtf8(replyArray);

    Log::instance().log("RequestManager", "replyBody: " + replyBody);

    return RequestManager::ResultStatus::RES_SUCCESS;
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

void RequestManager::addPageToShortcutMenu(QString ip, quint16 port, QStringList pagesList) {

    if (pagesList.size() != 7) {
        return;
    }

    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_ADD_PAGE_TO_SHORTCUT_MENU);
    QJsonObject argsObj;
    argsObj.insert("l1", pagesList[0]);
    argsObj.insert("l2", pagesList[1]);
    argsObj.insert("l3", pagesList[2]);
    argsObj.insert("l4", pagesList[3]);
    argsObj.insert("l5", pagesList[4]);
    argsObj.insert("l6", pagesList[5]);
    argsObj.insert("l7", pagesList[6]);
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

void RequestManager::setReadEnabledCOM1(QString ip, quint16 port)
{
    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_ID_SET_READ_ENABLED_SERIAL_1);
    QJsonObject argsObj;
    argsObj.insert("enabled", true);
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

void RequestManager::setReadDisabledCOM1(QString ip, quint16 port)
{
    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_ID_SET_READ_ENABLED_SERIAL_1);
    QJsonObject argsObj;
    argsObj.insert("enabled", false);
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

void RequestManager::setReadEnabledCOM2(QString ip, quint16 port)
{
    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_ID_SET_READ_ENABLED_SERIAL_2);
    QJsonObject argsObj;
    argsObj.insert("enabled", true);
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

void RequestManager::setReadDisabledCOM2(QString ip, quint16 port)
{
    QJsonObject bodyObj;
    bodyObj.insert("cmd", REQUEST_CMD_ID_SET_READ_ENABLED_SERIAL_2);
    QJsonObject argsObj;
    argsObj.insert("enabled", false);
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
