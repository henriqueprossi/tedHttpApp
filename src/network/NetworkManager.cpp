#include "NetworkManager.h"
#include "ConnectionManager.h"

NetworkManager &NetworkManager::instance() {

    static NetworkManager singleton;
    return singleton;
}

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent} {

    m_connectionManager = new ConnectionManager(this);

    QObject::connect(
        m_connectionManager, &ConnectionManager::connected,
        this, &NetworkManager::connected);

    QObject::connect(
        m_connectionManager, &ConnectionManager::disconnected,
        this, &NetworkManager::disconnected);
}

NetworkManager::~NetworkManager() {

}
