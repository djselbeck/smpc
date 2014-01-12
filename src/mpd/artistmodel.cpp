#include "artistmodel.h"

ArtistModel::ArtistModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

ArtistModel::ArtistModel(QList<MpdArtist *> *list, ImageDatabase *DB, QObject *parent) : QAbstractListModel(parent)
{
    mEntries = list;
    mImgDB = DB;
}

ArtistModel::~ArtistModel()
{
    if( mEntries ) {
        qDeleteAll(*mEntries);
        delete(mEntries);
    }
}

int ArtistModel::rowCount(const QModelIndex &parent) const
{
    return mEntries->length();
}

QVariant ArtistModel::data(const QModelIndex &index, int role) const
{
    if(role==NameRole)
        return mEntries->at(index.row())->getName();
    else if(role==SectionRole)
        return mEntries->at(index.row())->getSection();
    else if (role == ImageURLRole) {
        MpdArtist *tmpArtist = mEntries->at(index.row());
        int imageID = mImgDB->imageIDFromArtist(tmpArtist->getName());

        // No image found return dummy url
        if ( imageID == -1 ) {
            // Start image retrieval
            qDebug() << "returning dummy image for artist: " << tmpArtist->getName();
            //emit requestAlbumInformation(*album);
            // Return dummy for the time being
            return "";
        } else if (imageID == -2 ) {
            qDebug() << "returning dummy image for blacklisted artist: " << tmpArtist->getName();
            return "";
        } else {
            qDebug() << "returning database image for album: " << tmpArtist->getName();
            QString url = "image://imagedbprovider/artistid/" + QString::number(imageID);
            return url;
        }
    }
}

QHash<int, QByteArray> ArtistModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[NameRole] = "artist";
    roles[SectionRole] = "sectionprop";
    roles[ImageURLRole] = "imageURL";
    return roles;
}


MpdArtist *ArtistModel::get(int index)
{
    MpdArtist *retArtist = mEntries->at(index);
    QQmlEngine::setObjectOwnership(retArtist,QQmlEngine::CppOwnership);
    return retArtist;
}
