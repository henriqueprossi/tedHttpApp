#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H

#include <QObject>
#include <QJsonObject>

class QTcpServer;

class CommandServer : public QObject {

    Q_OBJECT

public:
    explicit CommandServer(QObject *parent = nullptr);
    ~CommandServer();

private:
    enum privateConstants {
        TCP_SERVER_PORT = 8090,
        TCP_SEND_TIMEOUT_MS = 5000
    };

    QTcpServer *m_tcpServer;

    void serverInit();
    QJsonObject processCommand(const QString tedIp, const QJsonObject &commandJson);

signals:
    void textFromTed(QString tedIp, qint8 source, QString text);

private slots:
    void handleNewConnection();
};

#endif // COMMAND_SERVER_H
