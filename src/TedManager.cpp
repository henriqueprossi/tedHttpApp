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

TedManager::TedManager(QObject *parent)
    : QObject(parent) {

    Log::instance().log("TedManager", "constructor");

    connectionManagerInit();
    requestManagerInit();
}

void TedManager::processReplyReceived(QString ip, QString body) {

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
    connect(m_reqManager, &RequestManager::replyReceived, this, &TedManager::processReplyReceived);
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