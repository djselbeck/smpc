#include "artistmodel.h"

ArtistModel::ArtistModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

ArtistModel::ArtistModel(QList<MpdArtist *> *list, QObject *parent) :QAbstractListModel(parent)
{
    m_entries = list;
    QHash<int,QByteArray> roles;
    roles[NameRole] = "artist";

    roles[SectionRole] = "sectionprop";
    setRoleNames(roles);
}

int ArtistModel::rowCount(const QModelIndex &parent) const
{
    return m_entries->length();
}

QVariant ArtistModel::data(const QModelIndex &index, int role) const
{
    if(role==NameRole)
        return m_entries->at(index.row())->getName();
    if(role==SectionRole)
        return m_entries->at(index.row())->getSection();
}
