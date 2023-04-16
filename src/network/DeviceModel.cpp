#include "DeviceModel.h"

Device::Device(const QString &ip)
    : m_ip(ip) {

}

QString Device::ip() const {

    return m_ip;
}

Device DeviceModel::get(int index) {

    if ((index < 0) || (index > m_devices.count())) {
        return Device("");
    }
    return m_devices.at(index);
}

DeviceModel::DeviceModel(QObject *parent)
    : QAbstractListModel(parent) {

}

DeviceModel::~DeviceModel() {

}

void DeviceModel::insert(int index, const QString &ip) {

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

//void DeviceModel::addDevice(const Device &device) {

//    beginInsertRows(QModelIndex(), rowCount(), rowCount());
//    m_devices << device;
//    endInsertRows();
//}

void DeviceModel::append(const QString &ip) {

    insert(count(), ip);
}

void DeviceModel::remove(int index) {

    if ((index < 0) || (index >= m_devices.count())) {
        return;
    }

    emit beginRemoveRows(QModelIndex(), index, index);
    m_devices.removeAt(index);
    emit endRemoveRows();

    // Update our count property.
    emit countChanged(m_devices.count());
}

int DeviceModel::rowCount(const QModelIndex &parent) const {

    Q_UNUSED(parent);
    return m_devices.count();
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const {

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

QHash<int, QByteArray> DeviceModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[IpRole] = "ip";
    return roles;
}
