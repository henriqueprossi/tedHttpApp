#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class RequestResult {
public:
    enum class ResultStatus {
        RES_SUCCESS,
        RES_FAIL
    };

    explicit RequestResult(const RequestResult::ResultStatus status, const QByteArray data);
    ~RequestResult();

private:
    RequestResult::ResultStatus m_status;
    QByteArray m_data;
};

class Request : public QObject {

    Q_OBJECT

public:
    explicit Request(QString ip, quint16 port, QString body, QObject *parent = nullptr);
    virtual ~Request() {};

    RequestResult::ResultStatus send(QString &replyBody);

private:
    enum {
        REQUEST_TIMEOUT_MS = 30000
    };

    QNetworkAccessManager *m_netManager;
    QString m_ip;
    quint16 m_port;
    QString m_body;

signals:

public slots:
    //void replyFinished();
    //void replyError(QNetworkReply::NetworkError error);

    //void replyReceived(QString body);
};

#endif // REQUEST_H
