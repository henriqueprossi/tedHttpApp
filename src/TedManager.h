#ifndef TEDMANAGER_H
#define TEDMANAGER_H

#include <QObject>
#include <QThread>

class ConnectionManager;
class RequestManager;
class CommandServer;

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
    Q_INVOKABLE void beepInit(QString ip, quint16 port);
    Q_INVOKABLE void beeps(QString ip, quint16 port, quint8 qtty);
    Q_INVOKABLE void sendToCOM1(QString ip, quint16 port, QString text);
    Q_INVOKABLE void sendToCOM2(QString ip, quint16 port, QString text);
    Q_INVOKABLE void readDigitalInput(QString ip, quint16 port);
    Q_INVOKABLE void turnOnDigitalInput(QString ip, quint16 port);
    Q_INVOKABLE void turnOffDigitalInput(QString ip, quint16 port);
    Q_INVOKABLE void clearShortcutMenu(QString ip, quint16 port);
    Q_INVOKABLE void addPageToShortcutMenu(QString ip, quint16 port, QStringList pagesList);
    Q_INVOKABLE void setReadEnabledCOM1(QString ip, quint16 port);
    Q_INVOKABLE void setReadDisabledCOM1(QString ip, quint16 port);
    Q_INVOKABLE void setReadEnabledCOM2(QString ip, quint16 port);
    Q_INVOKABLE void setReadDisabledCOM2(QString ip, quint16 port);

private:
    explicit TedManager(QObject *parent = nullptr);
    ~TedManager();

    void connectionManagerInit();
    void requestManagerInit();
    void commandSeverInit();

    void connectionManagerDeInit();
    void requestManagerDeInit();

    TedManager(const TedManager&) = delete;
    const TedManager& operator=(const TedManager&) = delete;

    ConnectionManager *m_connectionManager;
    QThread m_connManagerThread;

    RequestManager *m_reqManager;
    QThread m_reqManagerThread;

    CommandServer *m_commandServer;
    QThread m_commandServerThread;

public slots:
    void processReplyReceived(QString ip, QString body);
    void processTextFromTed(QString tedIp, qint8 source, QString text);

signals:
    void connected(QString ip);
    void disconnected(QString ip);

    void dataReceivedFrom(QString ip);
    void replyReceived(QString ip, QString body);
    void replyTimeout(QString ip);

    void replyReceivedReadDigitalInput(quint8 value);

    void textFromTed(QString tedIp, qint8 source, QString text);

    // Requests relative to Connection API.
    void reqConnectionStart();
    void reqConnectionStop();

    // Requests relative to Commands API.
    void reqSendTextToTed(QString ip, quint16 port, QString text);
    void reqClearDisplay(QString ip, quint16 port);
    void reqBeepInit(QString ip, quint16 port);
    void reqBeeps(QString ip, quint16 port, quint8 qtty);
    void reqSendToCOM1(QString ip, quint16 port, QString text);
    void reqSendToCOM2(QString ip, quint16 port, QString text);
    void reqReadDigitalInput(QString ip, quint16 port);
    void reqTurnOnDigitalInput(QString ip, quint16 port);
    void reqTurnOffDigitalInput(QString ip, quint16 port);
    void reqClearShortcutMenu(QString ip, quint16 port);
    void reqAddPageToShortcutMenu(QString ip, quint16 port, QStringList pagesList);
    void reqSetReadEnabledCOM1(QString ip, quint16 port);
    void reqSetReadDisabledCOM1(QString ip, quint16 port);
    void reqSetReadEnabledCOM2(QString ip, quint16 port);
    void reqSetReadDisabledCOM2(QString ip, quint16 port);
};

#endif // TEDMANAGER_H
