#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <QObject>
#include <QTimer>

class RequestManager : public QObject {

    Q_OBJECT

public:
    explicit RequestManager(QObject *parent = nullptr);
    ~RequestManager();

private:
    enum {
        REQUEST_TIMEOUT_DEF_MS = 10000
    };

signals:
    void replyReceived(QString ip, QString body);
    void replyTimeout(QString ip);

public slots:
    void sendTextToTed(QString ip, quint16 port, QString text);
    void clearDisplay(QString ip, quint16 port);
};

#endif // REQUEST_MANAGER_H
