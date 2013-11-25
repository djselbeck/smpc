#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

PlaylistModel::PlaylistModel(QList<MpdTrack *> *list,QObject *parent) : QAbstractListModel(parent)
{
       m_entries = list;
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if(role==NameRole)
    {
        return m_entries->at(index.row())->getName();
    }
    else if(role==pathRole)
        return m_entries->at(index.row())->getFileUri();
    else if(role==filenameRole)
        return m_entries->at(index.row())->getFileName();
    else if(role==titleRole)
        return m_entries->at(index.row())->getTitle();
    else if(role==artistRole)
        return m_entries->at(index.row())->getArtist();
    else if(role==albumRole)
        return m_entries->at(index.row())->getAlbum();
    else if(role==lengthRole)
        return m_entries->at(index.row())->getLength();
    else if(role==lengthFormatedRole)
        return m_entries->at(index.row())->getLengthFormated();
    else if(role==tracknoRole)
        return m_entries->at(index.row())->getTrackNr();
    else if(role==yearRole)
        return m_entries->at(index.row())->getYear();
    else if(role==playingRole)
        return m_entries->at(index.row())->getPlaying();
}

int PlaylistModel::rowCount(const QModelIndex &parent) const{
    return m_entries->length();
}

QHash<int, QByteArray> PlaylistModel::roleNames() const {
    QHash<int,QByteArray> roles;

    roles[NameRole] = "name";
    roles[pathRole] = "path";
    roles[filenameRole] = "filename";
    roles[titleRole] = "title";
    roles[artistRole] = "artist";
    roles[albumRole] = "album";
    roles[lengthRole] = "length";
    roles[lengthFormatedRole] = "lengthformated";
    roles[tracknoRole] = "tracknr";
    roles[yearRole] = "year";
    roles[playingRole] = "playing";

    return roles;
}

void PlaylistModel::setPlaying(int position, bool playing) {
    if(position<m_entries->length()) {
        m_entries->at(position)->setPlaying(playing);
        emit dataChanged(createIndex(position,0),createIndex(position,0),QVector<int>(1,playingRole));
    }
}

bool PlaylistModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if(role == playingRole) {
        m_entries->at(index.row())->setPlaying(value.toBool());
        return true;
    }
    return false;
}
