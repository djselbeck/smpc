#include "filemodel.h"

FileModel::FileModel(QObject *parent) :
    QAbstractListModel(parent)
{
}


FileModel::FileModel(QList<MpdFileEntry *> *list,QObject *parent) : QAbstractListModel(parent)
{
       m_entries = list;
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if(role==NameRole)
    {
        return m_entries->at(index.row())->getName();
    }
    else if(role==SectionRole)
        return m_entries->at(index.row())->getSection();
    else if(role==isDirectoryRole)
        return m_entries->at(index.row())->isDirectory();
    else if(role==isFileRole)
        return m_entries->at(index.row())->isFile();
    else if(role==isPlaylistRole)
        return m_entries->at(index.row())->isPlaylist();
    else if(role==pathRole)
        return m_entries->at(index.row())->getPath();
    else if(role==prePathRole)
        return m_entries->at(index.row())->getPrePath();
    else if(role==titleRole)
        return m_entries->at(index.row())->getTitle();
    else if(role==artistRole)
        return m_entries->at(index.row())->getArtist();
    else if(role==albumRole)
        return m_entries->at(index.row())->getAlbum();
    else if(role==lengthRole)
        return m_entries->at(index.row())->getLengthFormatted();
    else if(role==tracknoRole)
        return m_entries->at(index.row())->getTrackNr();
    else if(role==yearRole)
        return m_entries->at(index.row())->getYear();
}

int FileModel::rowCount(const QModelIndex &parent) const{
    return m_entries->length();
}

QHash<int, QByteArray> FileModel::roleNames() const {
    QHash<int,QByteArray> roles;

    roles[NameRole] = "name";
    roles[SectionRole] = "sectionprop";
    roles[isDirectoryRole] = "isDirectory";
    roles[isFileRole] = "isFile";
    roles[isPlaylistRole] = "isPlaylist";
    roles[prePathRole] = "prepath";
    roles[pathRole] = "path";
    roles[titleRole] = "title";
    roles[artistRole] = "artist";
    roles[albumRole] = "album";
    roles[lengthRole] = "length";
    roles[tracknoRole] = "tracknr";
    roles[yearRole] = "year";

    return roles;
}
