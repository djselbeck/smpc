#include "albummodel.h"

AlbumModel::AlbumModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

AlbumModel::AlbumModel(QList<MpdAlbum *> *list,QObject *parent) : QAbstractListModel(parent)
{
       m_entries = list;
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    if(role==AlbumRole)
    {
        return m_entries->at(index.row())->getTitle();
    }
    else if(role==SectionRole)
    {
        return m_entries->at(index.row())->getSection();
    }
    else if(role==ArtistRole)
    {
        return m_entries->at(index.row())->getArtist();
    }
    else if(role=AlbumCleandRole)
    {
        QString cleanedAlbum = m_entries->at(index.row())->getTitle();
        cleanedAlbum = cleanedAlbum.replace('/',"");
        return cleanedAlbum;
    }
}

int AlbumModel::rowCount(const QModelIndex &parent) const{
    return m_entries->length();
}

QHash<int, QByteArray> AlbumModel::roleNames() const {
    QHash<int,QByteArray> roles;

    roles[AlbumRole] = "title";
    roles[SectionRole] = "sectionprop";
    roles[ArtistRole] = "artist";
    roles[AlbumCleandRole] = "titleClean";
    return roles;
}
