#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

class ConnectionManager;

class NetworkManager : public QObject {

    Q_OBJECT

public:
    static NetworkManager& instance();

private:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    NetworkManager(const NetworkManager&) = delete;
    const NetworkManager& operator=(const NetworkManager&) = delete;

    ConnectionManager *m_connectionManager;

signals:
    void connected(QString ip);
    void disconnected(QString ip);
};

#endif // NETWORKMANAGER_H
