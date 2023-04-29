#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H

#include <QObject>
#include <QHttpServer>
#include <QJsonObject>

class CommandServer : public QObject {

    Q_OBJECT

public:
    explicit CommandServer(QObject *parent = nullptr);
    ~CommandServer();

private:
    enum privateConstants {
        HTTP_SERVER_PORT = 8090
    };

    QHttpServer m_httpServer;

    void httpServerInit();
    QJsonObject processCommand(const QJsonObject &commandJson);

signals:
    void textFromTed(qint8 source, QString text);

public slots:

};

#endif // COMMAND_SERVER_H
