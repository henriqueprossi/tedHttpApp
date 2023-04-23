#include "RequestSendTextToTed.h"

RequestSendTextToTed::RequestSendTextToTed(QString ip, quint16 port, QString text, QObject *parent)
    : Request(ip, port, text, parent) {

}

RequestSendTextToTed::~RequestSendTextToTed() {

}
