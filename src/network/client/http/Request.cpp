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

//void Request::replyFinished() {

//}
