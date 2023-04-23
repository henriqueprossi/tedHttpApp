#include "Log.h"
#include <QDebug>

Log &Log::instance() {

    static Log singleton;
    return singleton;
}

void Log::log(QString tag, QString text) {

    QDebug debug = qDebug();
    debug.noquote();
    debug.nospace();
    debug << "[" << tag << "] " << text;
}

Log::Log(QObject *parent) : QObject(parent) {

    //qDebug() << "[Log] constructor";
}

Log::~Log() {

    //qDebug() << "[Log] destructor";
}
