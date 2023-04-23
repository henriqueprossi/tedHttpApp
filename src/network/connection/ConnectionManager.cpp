#include "ConnectionManager.h"
#include "Log.h"
#include <QUdpSocket>

ConnectedDevice::ConnectedDevice(const QString ip, const quint32 keepAliveTimeoutMs, QObject *parent) :
    QObject(parent) {

    Log::instance().log("ConnectedDevice", "constructor");

    m_deviceIp = ip;
    m_keepAliveTimeoutMs = keepAliveTimeoutMs;

    m_keepAliveTimer = new QTimer(this);

    QObject::connect(m_keepAliveTimer, &QTimer::timeout, this, [this, ip]() {
        emit disconnected();
    });
}

ConnectedDevice::~ConnectedDevice() {

    Log::instance().log("ConnectedDevice", "destructor");
}

QString ConnectedDevice::getIp() const {

    return m_deviceIp;
}

void ConnectedDevice::refreshKeepAliveTimeout() {

    m_keepAliveTimer->start(m_keepAliveTimeoutMs);
}

ConnectionManager::ConnectionManager(QObject *parent)
    : QObject(parent) {

    Log::instance().log("ConnectionManager", "constructor");

    m_udpSocket = nullptr;
    m_connectedDevices.clear();
}

ConnectionManager::~ConnectionManager() {

    Log::instance().log("ConnectionManager", "destructor");

    stop();
}

bool ConnectionManager::sendConnectedReply(const QHostAddress &host, quint16 port) {

    QByteArray data;
    data.append("    Conectado");
    data[0] = 0x20;
    data[3] = 0x09;

    qint64 sent = m_udpSocket->writeDatagram(data, host, port);

    if (sent == data.size()) {
        return true;
    }
    return false;
}

void ConnectionManager::addConnectedDevice(const QString ip) {

    ConnectedDevice *device = new ConnectedDevice(ip, WAIT_KEEP_ALIVE_TIMEOUT_MS);

    QObject::connect(device, &ConnectedDevice::disconnected, this, [this, device]() {
        emit disconnected(device->getIp());
        m_connectedDevices.removeAll(device);
        delete device;
    });

    m_connectedDevices.append(device);

    device->refreshKeepAliveTimeout();

    emit connected(ip);
}

void ConnectionManager::disconnectAllDevices() {

    for (const auto &device : m_connectedDevices) {
        emit disconnected(device->getIp());
        delete device;
    }

    m_connectedDevices.clear();
}

ConnectedDevice *ConnectionManager::findConnectedDevice(const QString ip) {

    for (const auto device : m_connectedDevices) {
        if (device->getIp() == ip) {
            return device;
        }
    }
    return nullptr;
}

void ConnectionManager::readyRead() {

    QByteArray Buffer;
    Buffer.resize(m_udpSocket->pendingDatagramSize());

    QHostAddress senderIpV6;
    quint16 senderPort;
    m_udpSocket->readDatagram(Buffer.data(), Buffer.size(), &senderIpV6, &senderPort);

    bool ipOk = false;
    QHostAddress senderIpV4(senderIpV6.toIPv4Address(&ipOk));

    if (!ipOk) {
        return;
    }

    if (sendConnectedReply(senderIpV4, senderPort)) {
        QString senderIp = senderIpV4.toString();
        ConnectedDevice *device = findConnectedDevice(senderIp);
        // Check if the device is already present in the list. Ignore it if so.
        if (device == nullptr) {
            addConnectedDevice(senderIp);
        } else {
            device->refreshKeepAliveTimeout();
        }
    }
}

void ConnectionManager::start() {

    Log::instance().log("ConnectionManager", "start");

    if (m_udpSocket != nullptr) {
        Log::instance().log("ConnectionManager", "udp socket already created!");
        return;
    }

    Log::instance().log("ConnectionManager", "starting UDP socket...");

    m_udpSocket = new QUdpSocket();

    QObject::connect(m_udpSocket, &QUdpSocket::readyRead, this, &ConnectionManager::readyRead);

    // Bind the UDP socket to an address and a port
    m_udpSocket->bind(QHostAddress::Any, CONNECTION_PORT);
}

void ConnectionManager::stop() {

    Log::instance().log("ConnectionManager", "stop");

    if (m_udpSocket == nullptr) {
        Log::instance().log("ConnectionManager", "udp socket not created yet!");
        return;
    }

    Log::instance().log("ConnectionManager", "stopping UDP socket...");

    m_udpSocket->close();

    disconnectAllDevices();

    delete m_udpSocket;
    m_udpSocket = nullptr;
}

void ConnectionManager::processDataReceivedFrom(QString ip) {

    ConnectedDevice *device = findConnectedDevice(ip);
    if (device != nullptr) {
        device->refreshKeepAliveTimeout();
    }
}
