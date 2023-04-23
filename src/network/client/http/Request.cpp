#include <QEventLoop>
#include <QTimer>
#include "Request.h"
#include "Log.h"

RequestResult::RequestResult(const ResultStatus status, const QByteArray data)
    : m_status(status), m_data(data) {

}

RequestResult::~RequestResult() {

}

Request::Request(QString ip, quint16 port, QString body, QObject *parent)
    : m_ip(ip), m_port(port), m_body(body), QObject(parent) {

    m_netManager = new QNetworkAccessManager(this);

    //connect(m_netManager, &QNetworkAccessManager::finished,
    //        this, &Request::replyFinished);
}

RequestResult::ResultStatus Request::send(QString &replyBody) {

    QEventLoop eventLoop;
    QTimer timeoutTimer;
    bool timeout = false;

    QNetworkRequest request;
    QString url = "http://" + m_ip + ":" + QString::number(m_port);
    request.setUrl(QUrl(url));
    request.setRawHeader("User-Agent", "tedHttpApp");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(m_netManager, &QNetworkAccessManager::finished,
            &eventLoop, &QEventLoop::quit);

    connect(&timeoutTimer, &QTimer::timeout, this, [&]() {
        timeout = true;
        eventLoop.quit();
    });

    timeoutTimer.setInterval(REQUEST_TIMEOUT_MS);
    timeoutTimer.setSingleShot(true);
    timeoutTimer.start();

    QByteArray bodyArray = m_body.toUtf8();

    QNetworkReply *reply = m_netManager->post(request, bodyArray);

    eventLoop.exec();

    //connect(reply, &QIODevice::readyRead, this, &Request::replyFinished);
    //connect(reply, &QNetworkReply::errorOccurred, this, &Request::replyError);

    QNetworkReply::NetworkError result = reply->error() ;

    if (timeout || (result != QNetworkReply::NoError)) {

        Log::instance().log("Request", "Reply error! error: " + QString::number(result) + ", timeout: " + QString::number(timeout));

        return RequestResult::ResultStatus::RES_FAIL;

    } else {

        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        replyBody = QString(reply->readAll());

        Log::instance().log("Request", "Reply success! status code: " + QString::number(code) + ", body: " + replyBody);

        //emit replyReceived(replyBody);

        return RequestResult::ResultStatus::RES_SUCCESS;
    }
}

//void Request::replyFinished() {

//}
