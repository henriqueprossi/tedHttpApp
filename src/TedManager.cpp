#include "TedManager.h"
#include "ConnectionManager.h"
#include "RequestManager.h"
#include "Log.h"
//#include <QtConcurrent/QtConcurrentRun>

TedManager &TedManager::instance() {

    static TedManager singleton;
    return singleton;
}

void TedManager::startMonitoringConnection() {

    Log::instance().log("TedManager", "starting monitoring connection...");

    emit reqConnectionStart();

//    QFuture<void> future = QtConcurrent::run([this] {
//        m_connectionManager->start();
//    });
}

void TedManager::stopMonitoringConnection() {

    Log::instance().log("TedManager", "stoping monitoring connection...");

    emit reqConnectionStop();

//    QFuture<void> future = QtConcurrent::run([this] {
//        m_connectionManager->stop();
    //    });
}

void TedManager::sendTextToTed(QString ip, quint16 port, QString text) {

    emit reqSendTextToTed(ip, port, text);
}

void TedManager::clearDisplay(QString ip, quint16 port) {

    emit reqClearDisplay(ip, port);
}

void TedManager::beepInit(QString ip, quint16 port) {

    emit reqBeepInit(ip, port);
}

void TedManager::beeps(QString ip, quint16 port, quint8 qtty) {

    emit reqBeeps(ip, port, qtty);
}

void TedManager::sendToCOM1(QString ip, quint16 port, QString text) {

    emit reqSendToCOM1(ip, port, text);
}

void TedManager::sendToCOM2(QString ip, quint16 port, QString text) {

    emit reqSendToCOM2(ip, port, text);
}

void TedManager::readDigitalInput(QString ip, quint16 port) {

    emit reqReadDigitalInput(ip, port);
}

void TedManager::turnOnDigitalInput(QString ip, quint16 port) {

    emit reqTurnOnDigitalInput(ip, port);
}

void TedManager::turnOffDigitalInput(QString ip, quint16 port) {

    emit reqTurnOffDigitalInput(ip, port);
}

void TedManager::clearShortcutMenu(QString ip, quint16 port) {

    emit reqClearShortcutMenu(ip, port);
}

void TedManager::addPageToShortcutMenu(QString ip, quint16 port, QStringList pagesList) {

    emit reqAddPageToShortcutMenu(ip, port, pagesList);
}

TedManager::TedManager(QObject *parent)
    : QObject(parent) {

    Log::instance().log("TedManager", "constructor");

    connectionManagerInit();
    requestManagerInit();
}

void TedManager::processReplyReceived(QString ip, QString body) {

    emit replyReceived(ip, body);
    emit dataReceivedFrom(ip);
}

TedManager::~TedManager() {

    Log::instance().log("TedManager", "destructor");

    connectionManagerDeInit();
    requestManagerDeInit();
}

void TedManager::connectionManagerInit() {

    m_connectionManager = new ConnectionManager();
    m_connectionManager->moveToThread(&m_connManagerThread);

    connect(&m_connManagerThread, &QThread::finished, m_connectionManager, &QObject::deleteLater);
    connect(this, &TedManager::reqConnectionStart, m_connectionManager, &ConnectionManager::start);
    connect(this, &TedManager::reqConnectionStop, m_connectionManager, &ConnectionManager::stop);
    connect(this, &TedManager::dataReceivedFrom, m_connectionManager, &ConnectionManager::processDataReceivedFrom);
    //connect(m_connectionManager, &Worker::resultReady, this, &Controller::handleResults);
    m_connManagerThread.start();

    connect(m_connectionManager, &ConnectionManager::connected, this, &TedManager::connected);
    connect(m_connectionManager, &ConnectionManager::disconnected, this, &TedManager::disconnected);
}

void TedManager::requestManagerInit() {

    m_reqManager = new RequestManager();
    m_reqManager->moveToThread(&m_reqManagerThread);

    connect(&m_reqManagerThread, &QThread::finished, m_reqManager, &QObject::deleteLater);
    connect(this, &TedManager::reqSendTextToTed, m_reqManager, &RequestManager::sendTextToTed);
    connect(this, &TedManager::reqClearDisplay, m_reqManager, &RequestManager::clearDisplay);
    connect(this, &TedManager::reqBeepInit, m_reqManager, &RequestManager::beepInit);
    connect(this, &TedManager::reqBeeps, m_reqManager, &RequestManager::beeps);
    connect(this, &TedManager::reqSendToCOM1, m_reqManager, &RequestManager::sendToCOM1);
    connect(this, &TedManager::reqSendToCOM2, m_reqManager, &RequestManager::sendToCOM2);
    connect(this, &TedManager::reqReadDigitalInput, m_reqManager, &RequestManager::readDigitalInput);
    connect(this, &TedManager::reqTurnOnDigitalInput, m_reqManager, &RequestManager::turnOnDigitalInput);
    connect(this, &TedManager::reqTurnOffDigitalInput, m_reqManager, &RequestManager::turnOffDigitalInput);
    connect(this, &TedManager::reqClearShortcutMenu, m_reqManager, &RequestManager::clearShortcutMenu);
    connect(this, &TedManager::reqAddPageToShortcutMenu, m_reqManager, &RequestManager::addPageToShortcutMenu);
    connect(m_reqManager, &RequestManager::replyReceived, this, &TedManager::processReplyReceived);
    connect(m_reqManager, &RequestManager::replyTimeout, this, &TedManager::replyTimeout);
    connect(m_reqManager, &RequestManager::replyReceivedReadDigitalInput, this, &TedManager::replyReceivedReadDigitalInput);
    m_reqManagerThread.start();
}

void TedManager::connectionManagerDeInit() {

    m_connManagerThread.quit();
    m_connManagerThread.wait();
}

void TedManager::requestManagerDeInit() {

    m_reqManagerThread.quit();
    m_reqManagerThread.wait();
}
