#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>

class QUdpSocket;

class ConnectedDevice : public QObject {

    Q_OBJECT

public:
    ConnectedDevice(const QString ip, const quint32 keepAliveTimeoutMs, QObject *parent = nullptr);
    ~ConnectedDevice();

    void refreshKeepAliveTimeout();
    QString getIp() const;

private:
    QString m_deviceIp;
    quint32 m_keepAliveTimeoutMs;
    QTimer *m_keepAliveTimer;

signals:
    void disconnected();
};

class ConnectionManager : public QObject {

    Q_OBJECT

public:
    explicit ConnectionManager(QObject *parent = nullptr);
    ~ConnectionManager();

private:
    enum {
        KEEP_ALIVE_TIMEOUT_MS = 10000
    };

    QUdpSocket *m_udpSocket;
    QList<ConnectedDevice *> m_connectedDevices;

    bool sendConnectedReply(const QHostAddress &host, quint16 port);

    void addConnectedDevice(const QString ip);
    ConnectedDevice *findConnectedDevice(const QString ip);

signals:
    void connected(QString ip);
    void disconnected(QString ip);

public slots:
    void readyRead();
};

#endif // CONNECTION_MANAGER_H
