#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <QObject>
#include <QTimer>

struct TedInfo {
    QString m_ip;
    quint16 m_port;
};

class RequestManager : public QObject {

    Q_OBJECT

public:
    explicit RequestManager(QObject *parent = nullptr);
    ~RequestManager();

private:
    enum {
        REQUEST_TIMEOUT_MS = 5000
    };

    enum {
        REQUEST_CMD_SEND_TEXT_TO_TED = 1,
        REQUEST_CMD_BEEP_INIT = 2,
        REQUEST_CMD_CLEAR_DISPLAY = 3,
        REQUEST_CMD_BEEPS = 5,
        REQUEST_CMD_SEND_TO_COM1 = 6,
        REQUEST_CMD_SEND_TO_COM2 = 7,
        REQUEST_CMD_READ_DIGITAL_INPUT = 13,
        REQUEST_CMD_TURN_ON_DIGITAL_INPUT = 14,
        REQUEST_CMD_TURN_OFF_DIGITAL_INPUT = 15,
        REQUEST_CMD_CLEAR_SHORTCUT_MENU = 17
    };

    enum class ResultStatus {
        RES_SUCCESS,
        RES_FAIL
    };

    RequestManager::ResultStatus send(TedInfo &tedInfo, QString &requestBody, QString &replyBody);

signals:
    void replyReceived(QString ip, QString body);
    void replyTimeout(QString ip);

    void replyReceivedReadDigitalInput(quint8 value);

public slots:
    void sendTextToTed(QString ip, quint16 port, QString text);
    void clearDisplay(QString ip, quint16 port);
    void beepInit(QString ip, quint16 port);
    void beeps(QString ip, quint16 port, quint8 qtty);
    void sendToCOM1(QString ip, quint16 port, QString text);
    void sendToCOM2(QString ip, quint16 port, QString text);
    void readDigitalInput(QString ip, quint16 port);
    void turnOnDigitalInput(QString ip, quint16 port);
    void turnOffDigitalInput(QString ip, quint16 port);
    void clearShortcutMenu(QString ip, quint16 port);
};

#endif // REQUEST_MANAGER_H
