#ifndef TEDCLIENTINTERFACE_H
#define TEDCLIENTINTERFACE_H

#include <QObject>

class TedInfo {
public:
    explicit TedInfo(const QString ip, const quint16 port)
        : m_ip(ip), m_port(port) {}
    ~TedInfo() {};

private:
    QString m_ip;
    quint16 m_port;
};

class TedClientInterface : public QObject {

    Q_OBJECT

public:
    explicit TedClientInterface(QObject *parent = nullptr);
    virtual ~TedClientInterface() {};

    //virtual bool sendText(TedInfo &tedInfo) = 0;

signals:

};

#endif // TEDCLIENTINTERFACE_H
