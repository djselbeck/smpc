#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

PlaylistModel::PlaylistModel(QList<MpdTrack *> *list,QObject *parent) : QAbstractListModel(parent)
{
    mEntries = list;
}

PlaylistModel::~PlaylistModel(){
    if(mEntries){
        qDeleteAll(*mEntries);
        delete(mEntries);
        mEntries = 0;
    }
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if(role==NameRole)
    {
        return mEntries->at(index.row())->getName();
    }
    else if(role==pathRole)
        return mEntries->at(index.row())->getFileUri();
    else if(role==filenameRole)
        return mEntries->at(index.row())->getFileName();
    else if(role==titleRole)
        return mEntries->at(index.row())->getTitle();
    else if(role==artistRole)
        return mEntries->at(index.row())->getArtist();
    else if(role==albumRole)
        return mEntries->at(index.row())->getAlbum();
    else if(role==lengthRole)
        return mEntries->at(index.row())->getLength();
    else if(role==lengthFormatedRole)
        return mEntries->at(index.row())->getLengthFormated();
    else if(role==tracknoRole)
        return mEntries->at(index.row())->getTrackNr();
    else if(role==yearRole)
        return mEntries->at(index.row())->getYear();
    else if(role==playingRole)
        return mEntries->at(index.row())->getPlaying();

    return 0;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const{
    if(mEntries)
        return mEntries->length();
    else
        return 0;
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
    if(position<mEntries->length()) {
        mEntries->at(position)->setPlaying(playing);
        emit dataChanged(createIndex(position,0),createIndex(position,0),QVector<int>(1,playingRole));
    }
}

bool PlaylistModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if(role == playingRole) {
        mEntries->at(index.row())->setPlaying(value.toBool());
        return true;
    }
    return false;
}

MpdTrack* PlaylistModel::get(int index) {
    MpdTrack *retTrack;
    QQmlEngine::setObjectOwnership(retTrack,QQmlEngine::CppOwnership);
    return retTrack;
}
