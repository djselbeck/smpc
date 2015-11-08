#include "serverprofilemodel.h"

ServerProfileModel::ServerProfileModel(QObject *parent) :
    QAbstractListModel(parent)
{
    mEntries = 0;
}

ServerProfileModel::ServerProfileModel(QList<ServerProfile *> *list, QObject *parent) : QAbstractListModel(parent)
{
    mEntries = list;
}

ServerProfileModel::~ServerProfileModel()
{
    if(mEntries){
        qDeleteAll(*mEntries);
        delete(mEntries);
    }
}


QVariant ServerProfileModel::data(const QModelIndex &index, int role) const
{
    if ( Q_UNLIKELY(index.row() < 0 || index.row() > rowCount())) {
        return QVariant(QVariant::Invalid);
    }
    if(role==ServerNameRole)
    {
        return mEntries->at(index.row())->getHostname();
    }
    else if(role==ProfileNameRole)
    {
        return mEntries->at(index.row())->getName();
    }
    else if(role==PasswordRole)
    {
        return mEntries->at(index.row())->getPassword();
    }
    else if(role==PortRole)
    {
        return mEntries->at(index.row())->getPort();
    }
    else if(role==AutoconnectRole)
    {
        return mEntries->at(index.row())->getAutoconnect();
    }
    else if(role==MACAddressRole)
    {
        return mEntries->at(index.row())->getMACAddress();
    }

    return QVariant(QVariant::Invalid);
}

int ServerProfileModel::rowCount(const QModelIndex &parent) const{
    if ( parent.isValid() ) {
        return 0;
    }
    return mEntries->length();
}

QHash<int, QByteArray> ServerProfileModel::roleNames() const {
    QHash<int,QByteArray> roles;

    roles[ServerNameRole] = "hostname";
    roles[ProfileNameRole] = "name";
    roles[PortRole] = "port";
    roles[PasswordRole] = "password";
    roles[AutoconnectRole] = "autoconnect";
    roles[MACAddressRole] = "macaddress";
    return roles;
}


ServerProfile *ServerProfileModel::get(int index)
{
    ServerProfile *retProfile = mEntries->at(index);
    QQmlEngine::setObjectOwnership(retProfile,QQmlEngine::CppOwnership);
    return retProfile;
}

void ServerProfileModel::notifyChanged(int position)
{
    QVector<int> roles;
    roles.append(ProfileNameRole);
    roles.append(ServerNameRole);
    roles.append(PasswordRole);
    roles.append(PortRole);
    roles.append(AutoconnectRole);
    roles.append(MACAddressRole);
    emit dataChanged(createIndex(position,0),createIndex(position,0),roles);
}

void ServerProfileModel::append(ServerProfile *profile)
{
    if ( profile == 0 || mEntries == 0) {
        return;
    }
    beginResetModel();
    mEntries->append(profile);
    endResetModel();
}

void ServerProfileModel::remove(int position)
{
    if ( mEntries!= 0 && position>=0 && position<mEntries->size()){
        beginResetModel();
        mEntries->removeAt(position);
        endResetModel();
    }
}
