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

    QString getIp() const;

private:
    QString m_deviceIp;
    quint32 m_keepAliveTimeoutMs;
    QTimer *m_keepAliveTimer;

public slots:
    void refreshKeepAliveTimeout();

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
        DEVICE_KEEP_ALIVE_TIMEOUT_MS = 10000,
        WAIT_KEEP_ALIVE_TIMEOUT_MS = 2 * DEVICE_KEEP_ALIVE_TIMEOUT_MS,
        CONNECTION_PORT = 55555
    };

    QUdpSocket *m_udpSocket;
    QList<ConnectedDevice *> m_connectedDevices;

    bool sendConnectedReply(const QHostAddress &host, quint16 port);

    void addConnectedDevice(const QString ip);
    void disconnectAllDevices();
    ConnectedDevice *findConnectedDevice(const QString ip);

signals:
    void connected(QString ip);
    void disconnected(QString ip);

public slots:
    void readyRead();
    void start();
    void stop();
    void processDataReceivedFrom(QString ip);
};

#endif // CONNECTION_MANAGER_H
