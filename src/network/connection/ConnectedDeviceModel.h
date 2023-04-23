#ifndef CONNECTEDDEVICEMODEL_H
#define CONNECTEDDEVICEMODEL_H

#include <QAbstractListModel>

class Device {

public:
    Device(const QString &ip);
    ~Device();

    QString ip() const;

private:
    QString m_ip;
};

class ConnectedDeviceModel : public QAbstractListModel {

    Q_OBJECT
    // Gives the size of the model.
    Q_PROPERTY(int count READ count NOTIFY countChanged);
    // Gets the ip from the device at the index
    Q_INVOKABLE Device get(int index);

public:
    enum DeviceRoles {
        IpRole = Qt::UserRole + 1
    };

    explicit ConnectedDeviceModel(QObject *parent = nullptr);
    ~ConnectedDeviceModel();

    Q_INVOKABLE void insert(int index, const QString &ip);
    Q_INVOKABLE void append(const QString &ip);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void remove(const QString &ip);

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    int count() const {
        return m_devices.count();
    }

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Device> m_devices;

signals:
    void countChanged(int count);
};

#endif // CONNECTEDDEVICEMODEL_H
