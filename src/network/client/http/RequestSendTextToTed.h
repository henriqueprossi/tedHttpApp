#ifndef REQUEST_SEND_TEXT_TO_TED_H
#define REQUEST_SEND_TEXT_TO_TED_H

#include "Request.h"

class RequestSendTextToTed : public Request {

    Q_OBJECT

public:
    explicit RequestSendTextToTed(QString ip, quint16 port, QString text, QObject *parent = nullptr);
    ~RequestSendTextToTed();

private:
    QString m_ip;
    quint16 m_port;
    QString m_text;

signals:

};

#endif // REQUEST_SEND_TEXT_TO_TED_H
