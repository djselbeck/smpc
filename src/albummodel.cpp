#include "albummodel.h"

AlbumModel::AlbumModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

AlbumModel::AlbumModel(QList<MpdAlbum *> *list, ImageDatabase *db, QObject *parent) : QAbstractListModel(parent)
{
       m_entries = list;
       mDB = db;
       mDownloader = new ImageDownloader();

       connect(this,SIGNAL(requestAlbumInformation(MpdAlbum)),mDownloader,SLOT(requestAlbumArt(MpdAlbum)),Qt::QueuedConnection);
       connect(mDownloader,SIGNAL(albumInformationReady(AlbumInformation*)),this,SLOT(albumInformationReady(AlbumInformation*)));

//       foreach(MpdAlbum *album, *list) {
//           emit requestAlbumInformation(album);
//       }

       //mImageProvider = new LastFMAlbumProvider("","");
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
    else if(role==AlbumCleandRole)
    {
        QString cleanedAlbum = m_entries->at(index.row())->getTitle();
        cleanedAlbum = cleanedAlbum.replace('/',"");
        return cleanedAlbum;
    }
    else if (role==AlbumImageRole)
    {
        MpdAlbum *album = m_entries->at(index.row());
        qDebug() << "Image for album: " << album->getTitle() << "requested";
        if ( album->getArtist() != "" ) {
            int imageID = mDB->imageIDFromAlbumArtist(album->getTitle(),album->getArtist());

            // No image found return dummy url
            if ( imageID == -1 ) {
                // Start image retrieval
                qDebug() << "returning dummy image for album: " << album->getTitle();
                emit requestAlbumInformation(*album);
                // Return dummy for the time being
                return DUMMY_ALBUMIMAGE;
            } else if (imageID == -2 ) {
                qDebug() << "returning dummy image for blacklisted album: " << album->getTitle();
                return DUMMY_ALBUMIMAGE;
            } else {
                qDebug() << "returning database image for album: " << album->getTitle();
                QString url = "image://imagedbprovider/albumid/" + QString::number(imageID);
                return url;
            }
        }
        else {
            int imageID = mDB->imageIDFromAlbum(album->getTitle());

            // No image found return dummy url
            if ( imageID == -1 ) {
                // Start image retrieval
                qDebug() << "returning dummy image for album: " << album->getTitle();
                // Return dummy for the time being
                return DUMMY_ALBUMIMAGE;
            } else if (imageID == -2 ) {
                qDebug() << "returning dummy image for blacklisted album: " << album->getTitle();
                return DUMMY_ALBUMIMAGE;
            }
            else {
                qDebug() << "returning database image for album: " << album->getTitle();
                QString url = "image://imagedbprovider/albumid/" + QString::number(imageID);
                return url;
            }
        }
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
    roles[AlbumImageRole] = "coverURL";
    return roles;
}

void AlbumModel::albumInformationReady(AlbumInformation *info)
{
    if( info == 0 ) {
        return;
    }
    QString album = info->getName();
    QString artist = info->getArtist();
    qDebug() << "received new information for album: " << album;

    // Search for it in entries
    for ( int i = 0; i < m_entries->length(); i++ ) {
        MpdAlbum *albumObj = m_entries->at(i);
        // Found corresponding albumObj, update coverimage url
        if (albumObj->getTitle() == album) {
            qDebug() << "entering new album information";
            mDB->enterAlbumInformation(info);
            emit dataChanged(createIndex(i,0),createIndex(i,0),QVector<int>(1,AlbumImageRole));
        }
    }
}
