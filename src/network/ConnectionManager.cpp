#include "ConnectionManager.h"
#include <QUdpSocket>

ConnectedDevice::ConnectedDevice(const QString ip, const quint32 keepAliveTimeoutMs, QObject *parent) :
    QObject(parent) {

    m_deviceIp = ip;
    m_keepAliveTimeoutMs = keepAliveTimeoutMs;

    m_keepAliveTimer = new QTimer(this);

    QObject::connect(m_keepAliveTimer, &QTimer::timeout, this, [this, ip]() {
        emit disconnected();
    });
}

ConnectedDevice::~ConnectedDevice() {

    qDebug() << "ConnectedDevice destructor";
}

void ConnectedDevice::refreshKeepAliveTimeout() {

    m_keepAliveTimer->start(m_keepAliveTimeoutMs);
}

QString ConnectedDevice::getIp() const {

    return m_deviceIp;
}

ConnectionManager::ConnectionManager(QObject *parent)
    : QObject{parent} {

    m_udpSocket = new QUdpSocket(this);

    // Bind the UDP socket to an address and a port
    m_udpSocket->bind(QHostAddress::Any, 55555);

    QObject::connect(m_udpSocket, &QUdpSocket::readyRead, this, &ConnectionManager::readyRead);
}

ConnectionManager::~ConnectionManager() {

    qDebug() << "ConnectionManager destructor";
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

    ConnectedDevice *device = new ConnectedDevice(ip, KEEP_ALIVE_TIMEOUT_MS);

    QObject::connect(device, &ConnectedDevice::disconnected, this, [this, device]() {
        emit disconnected(device->getIp());
        delete device;
    });

    m_connectedDevices.append(device);

    device->refreshKeepAliveTimeout();

    emit connected(ip);
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
