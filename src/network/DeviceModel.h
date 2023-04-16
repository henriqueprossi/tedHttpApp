#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QAbstractListModel>

class Device {

public:
    Device(const QString &ip);

    QString ip() const;

private:
    QString m_ip;
};

class DeviceModel : public QAbstractListModel {

    Q_OBJECT
    // Gives the size of the model.
    Q_PROPERTY(int count READ count NOTIFY countChanged);
    // Gets the ip from the device at the index
    Q_INVOKABLE Device get(int index);

public:
    enum DeviceRoles {
        IpRole = Qt::UserRole + 1
    };

    explicit DeviceModel(QObject *parent = nullptr);
    ~DeviceModel();

    void insert(int index, const QString &ip);
    void append(const QString &ip);
    void remove(int index);

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

#endif // DEVICEMODEL_H
