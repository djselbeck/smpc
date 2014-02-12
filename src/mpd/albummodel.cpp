#include "albummodel.h"

AlbumModel::AlbumModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

AlbumModel::AlbumModel(QList<MpdAlbum *> *list, ImageDatabase *db, QString lastfmartsize, QObject *parent) : QAbstractListModel(parent)
{
    mEntries = list;
    mDB = db;
    mDownloader = new ImageDownloader();
    mDownloader->setDownloadSize(lastfmartsize);

    connect(this,SIGNAL(requestAlbumInformation(MpdAlbum)),mDownloader,SLOT(requestAlbumArt(MpdAlbum)),Qt::QueuedConnection);
    connect(mDownloader,SIGNAL(albumInformationReady(AlbumInformation*)),this,SLOT(albumInformationReady(AlbumInformation*)));
    connect(this,SIGNAL(requestDBEnter(AlbumInformation*)),mDB,SLOT(enterAlbumInformation(AlbumInformation*)));
    connect(mDB,SIGNAL(albumEntered(QString)),this,SLOT(albumEntered(QString)));
}

AlbumModel::~AlbumModel()
{
    if(mEntries){
        qDeleteAll(*mEntries);
        delete(mEntries);
    }
    delete(mDownloader);
}


QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    if(role==AlbumRole)
    {
        return mEntries->at(index.row())->getTitle();
    }
    else if(role==SectionRole)
    {
        return mEntries->at(index.row())->getSection();
    }
    else if(role==ArtistRole)
    {
        return mEntries->at(index.row())->getArtist();
    }
    else if(role==AlbumCleandRole)
    {
        QString cleanedAlbum = mEntries->at(index.row())->getTitle();
        cleanedAlbum = cleanedAlbum.replace('/',"");
        return cleanedAlbum;
    }
    else if (role==AlbumImageRole)
    {
        MpdAlbum *album = mEntries->at(index.row());
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
                // Try getting album art for album with out artist (think samplers)
                imageID = mDB->imageIDFromAlbum(album->getTitle());
                if ( imageID >= 0 ) {
                    QString url = "image://imagedbprovider/albumid/" + QString::number(imageID);
                    return url;
                }
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
    return 0;
}

int AlbumModel::rowCount(const QModelIndex &parent) const{
    return mEntries->length();
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

    emit requestDBEnter(info);

}

void AlbumModel::albumEntered(QString albumName)
{
    qDebug() << "received new information for album: " << albumName;


    // Search for it in entries
    for ( int i = 0; i < mEntries->length(); i++ ) {
        MpdAlbum *albumObj = mEntries->at(i);
        // Found corresponding albumObj, update coverimage url
        if (albumObj->getTitle() == albumName) {
            emit dataChanged(createIndex(i,0),createIndex(i,0),QVector<int>(1,AlbumImageRole));
        }
    }
}

MpdAlbum *AlbumModel::get(int index)
{
    MpdAlbum *retAlbum = mEntries->at(index);
    QQmlEngine::setObjectOwnership(retAlbum,QQmlEngine::CppOwnership);
    return retAlbum;
}
