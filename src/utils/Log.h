#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QThread>

class ConnectionManager;

class Log : public QObject {

    Q_OBJECT

public:
    static Log& instance();

    Q_INVOKABLE void log(QString tag, QString text);

private:
    explicit Log(QObject *parent = nullptr);
    ~Log();

    Log(const Log&) = delete;
    const Log& operator=(const Log&) = delete;
};

#endif // LOG_H
