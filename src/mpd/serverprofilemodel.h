#ifndef SERVERPROFILEMODEL_H
#define SERVERPROFILEMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

#include <mpd/serverprofile.h>

class ServerProfileModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    explicit ServerProfileModel(QObject *parent = 0);
    ServerProfileModel(QList<ServerProfile*> *list, QObject *parent = 0);
    ~ServerProfileModel();

    Q_INVOKABLE ServerProfile* get(int index);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void notifyChanged(int position);
    Q_INVOKABLE void append(ServerProfile *profile);
    Q_INVOKABLE void remove(int position);

    enum EntryRoles {
        ServerNameRole = Qt::UserRole + 1,
        ProfileNameRole,
        PortRole,
        PasswordRole,
        MACAddressRole,
        AutoconnectRole
    };

private:
    QList<ServerProfile*> *mEntries;

signals:


public slots:


};

#endif // SERVERPROFILEMODEL_H
