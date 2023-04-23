#ifndef TEDMANAGER_H
#define TEDMANAGER_H

#include <QObject>
#include <QThread>

class ConnectionManager;
class RequestManager;

class TedManager : public QObject {

    Q_OBJECT

public:
    static TedManager& instance();

    // Connection API.
    Q_INVOKABLE void startMonitoringConnection();
    Q_INVOKABLE void stopMonitoringConnection();

    // Commands API.
    Q_INVOKABLE void sendTextToTed(QString ip, quint16 port, QString text);
    Q_INVOKABLE void clearDisplay(QString ip, quint16 port);

private:
    explicit TedManager(QObject *parent = nullptr);
    ~TedManager();

    void connectionManagerInit();
    void requestManagerInit();

    void connectionManagerDeInit();
    void requestManagerDeInit();

    TedManager(const TedManager&) = delete;
    const TedManager& operator=(const TedManager&) = delete;

    ConnectionManager *m_connectionManager;
    QThread m_connManagerThread;

    RequestManager *m_reqManager;
    QThread m_reqManagerThread;

public slots:
    void processReplyReceived(QString ip, QString body);

signals:
    void connected(QString ip);
    void disconnected(QString ip);

    void dataReceivedFrom(QString ip);

    // Requests relative to Connection API.
    void reqConnectionStart();
    void reqConnectionStop();

    // Requests relative to Commands API.
    void reqSendTextToTed(QString ip, quint16 port, QString text);
    void reqClearDisplay(QString ip, quint16 port);
};

#endif // TEDMANAGER_H
