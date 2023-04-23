#include "ConnectedDeviceModel.h"

Device::Device(const QString &ip)
    : m_ip(ip) {

}

Device::~Device() {

}

QString Device::ip() const {

    return m_ip;
}

Device ConnectedDeviceModel::get(int index) {

    if ((index < 0) || (index > m_devices.count())) {
        return Device("");
    }
    return m_devices.at(index);
}

ConnectedDeviceModel::ConnectedDeviceModel(QObject *parent)
    : QAbstractListModel(parent) {

}

ConnectedDeviceModel::~ConnectedDeviceModel() {

}

void ConnectedDeviceModel::insert(int index, const QString &ip) {

    if ((index < 0) || (index > m_devices.count())) {
        return;
    }

    // Create the device instance to be added to the list.
    Device newDevice(ip);

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_devices << newDevice;
    endInsertRows();

    emit countChanged(m_devices.count());
}

//void ConnectedDeviceModel::addDevice(const Device &device) {

//    beginInsertRows(QModelIndex(), rowCount(), rowCount());
//    m_devices << device;
//    endInsertRows();
//}

void ConnectedDeviceModel::append(const QString &ip) {

    insert(count(), ip);
}

void ConnectedDeviceModel::remove(int index) {

    if ((index < 0) || (index >= m_devices.count())) {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_devices.removeAt(index);
    endRemoveRows();

    // Update our count property.
    emit countChanged(m_devices.count());
}

void ConnectedDeviceModel::remove(const QString &ip) {

    for (int index = 0; index < m_devices.count(); index++) {
        if (m_devices.at(index).ip() == ip) {
            remove(index);
            break;
        }
    }
}

int ConnectedDeviceModel::rowCount(const QModelIndex &parent) const {

    Q_UNUSED(parent);
    return m_devices.count();
}

QVariant ConnectedDeviceModel::data(const QModelIndex &index, int role) const {

    // The index returns the requested row and column information. We ignore
    // the column and only use the row information.
    int row = index.row();

    // Boundary check for the row.
    if ((row < 0) || (row >= m_devices.count())) {
        return QVariant();
    }

    // A model can return data for different roles. The default role is the display role.
    // It can be accesses in QML with "model.display".

    const Device &device = m_devices[row];
    if (role == IpRole) {
        return device.ip();
    }

    // The view asked for other data, just return an empty QVariant.
    return QVariant();
}

QHash<int, QByteArray> ConnectedDeviceModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[IpRole] = "ip";
    return roles;
}
