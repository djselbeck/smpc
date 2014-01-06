#include "imagedatabase.h"

ImageDatabase::ImageDatabase(QObject *parent) :
    QObject(parent)
{
    mFiller = 0;
    QStringList drivers = QSqlDatabase::drivers();
    if ( !drivers.contains("QSQLITE") ) {
    } else {
        // Check if database exists already otherwise create on
        QDir dirAccess;
        if ( !dirAccess.mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation))) {
            return;
        }
        QString dbLocation = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/imgDB.sqlite3";
        mDB = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
        mDB->setDatabaseName(dbLocation);
        if ( mDB->open() ) {
            // Check if database contains necessary tables
            if ( !mDB->tables().contains("albums") ) {
                QSqlQuery createQuery;
                if (createQuery.exec("CREATE TABLE albums"
                                 "(id integer primary key AUTOINCREMENT,"
                                 "albumname text default '',"
                                 "artistname text default '',"
                                 "albuminfo blob '',"
                                 "imageID text )") ) {
                }
            }
            if ( !mDB->tables().contains("images") ) {
                QSqlQuery createQuery;
                if (createQuery.exec("CREATE TABLE images"
                                 "(id integer primary key AUTOINCREMENT,"
                                 "imghash text default '',"
                                 "imgdata blob)") ) {
                }
            }
            if ( !mDB->tables().contains("artist") ) {
                QSqlQuery createQuery;
                if (createQuery.exec("CREATE TABLE artists"
                                 "(id integer primary key,"
                                 "name text default '',"
                                 "artistinfo blob,"
                                 "imageID text )") ) {
                }
            }
        }
    }
    mDownloader = new ImageDownloader();
    qRegisterMetaType<MpdAlbum>("MpdAlbum");
    qRegisterMetaType<MpdArtist>("MpdArtist");

    connect(mDownloader,SIGNAL(albumInformationReady(AlbumInformation*)),this,SLOT(enterAlbumInformation(AlbumInformation*)));
    connect(this,SIGNAL(requestAlbumDownload(MpdAlbum)),mDownloader,SLOT(requestAlbumArt(MpdAlbum)));

    connect(mDownloader,SIGNAL(artistInformationReady(ArtistInformation*)),this,SLOT(enterArtistInformation(ArtistInformation*)));
    connect(this,SIGNAL(requestArtistDownload(MpdArtist)),mDownloader,SLOT(requestArtistArt(MpdArtist)));

    requestArtistBioInformation("Nightwish");
}

ImageDatabase::~ImageDatabase() {
    if ( mDB->isOpen()) {
        mDB->close();
    }
}

bool ImageDatabase::syncAlbums(QList<MpdAlbum*> *albums,MpdArtist *artist) {
    // Remove orphaned items
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT * FROM albums");
    query.exec();
    QStringList albumsToRemove;
    // Search orphaned albums
    while ( query.next() ) {
        QString albumName = query.value("albumname").toString();
        bool foundAlbum = false;
        for (QList<MpdAlbum*>::Iterator it = albums->begin();it != albums->end();it++) {
            if ( (*it)->getTitle() == albumName) {
                foundAlbum = true;
                break;
            }
        }
        if ( !foundAlbum ) {
            albumsToRemove.append(albumName);
        }
    }
    // remove orphaned albums
    for( int i = 0; i < albumsToRemove.length();i++) {
        QSqlQuery removeQuery;
        if ( removeQuery.exec("DELETE FROM albums where albumname = \"" +
                         albumsToRemove.at(i) +
                         "\"") ) {
        }
    }

    if ( albums->size() > 0 ) {
        mAlbumSyncRunning = true;
        mAlbums = albums;
        mAlbumNo = 0;
        mAlbumArtist = artist;
        MpdAlbum *firstAlbum = albums->first();
        //mCurrentAlbumProvider = new LastFMAlbumProvider(*firstAlbum,*mAlbumArtist);
        connect(mCurrentAlbumProvider,SIGNAL(ready(AlbumInformation*)),this,SLOT(albumReady(AlbumInformation*)));
        mCurrentAlbumProvider->startDownload();
    }

    return true;
}

bool ImageDatabase::syncArtists(QList<MpdAlbum*> &artists) {

    return true;
}

void ImageDatabase::albumReady(AlbumInformation *albumInformation) {
    QString albumName = albumInformation->getName();
    QString albumURL = albumInformation->getURL();
    QByteArray *imgData = albumInformation->getImageData();
    delete(mCurrentAlbumProvider);
    if ( mAlbumNo < mAlbums->size() ) {
        mAlbumNo++;
        MpdAlbum *album = mAlbums->at(mAlbumNo);
        //mCurrentAlbumProvider = new LastFMAlbumProvider(*album,*mAlbumArtist);
        connect(mCurrentAlbumProvider,SIGNAL(ready(AlbumInformation*)),this,SLOT(albumReady(AlbumInformation*)));
        mCurrentAlbumProvider->startDownload();
    }
}

void ImageDatabase::fillDatabase(QMap<MpdArtist*, QList<MpdAlbum*>* > *map)
{
    qDebug() << "Received artist/album map from netaccess, start bulk download";
    foreach (QList<MpdAlbum*> *albumList, *map ) {
        foreach (MpdAlbum *album, *albumList) {
            int imageID = imageIDFromAlbumArtist(album->getTitle(),album->getArtist());
            if ( imageID == -1 ) {
                emit requestAlbumDownload(MpdAlbum(this,album->getTitle(),album->getArtist()));
            }
            delete(album);
        }
    }
}

bool ImageDatabase::hasAlbumArt(QString album,QString artist)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM albums WHERE "
                  "albumname=\"" + album + "\" AND "
                  "artistname=\"" + artist + "\"");
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        QString albumName = query.value("albumname").toString();
        if ( albumName == album) {
            return true;
        }
    }
    return false;
}

bool ImageDatabase::hasArtistArt(QString artist)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM artist WHERE "
                  "artistname=\"" + artist + "\"");
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        QString artistName = query.value("artistname").toString();
        if ( artistName == artist) {
            return true;
        }
    }
    return false;
}

void ImageDatabase::enterAlbumInformation(AlbumInformation *info)
{
    qDebug() << "Entering album: " << info->getName() << " from: " << info->getArtist();
    // Check if album is already part of the database
    if ( hasAlbumArt(info->getName(),info->getArtist()) ) {
        qDebug() << "Album: " << info->getName() << " already part of database, skipping";
        // TODO check if image changed
        info->deleteLater();
        return;
    }

    // Check if Image has image data otherwise skip it
//    if ( !info->getImageData() ) {
//        qDebug() << "Album has no cover image, skipping";
//        return;
//    }

    // Check if image with same hash value is already part of the database
    int imgID = imageIDFromHash(info->getImageHash());

    // Enter Album into SQL DB
    if (imgID == -1 && info->getImageData() ) {
        QSqlQuery query;
        query.prepare("INSERT INTO images ("
                      "imghash, imgdata ) "
                      "VALUES ("
                      ":imghash, :imgdata)");
        query.bindValue(":imghash", info->getImageHash());
        if(info->getImageData()) {
            query.bindValue(":imgdata",*info->getImageData());
        } else {
            query.bindValue(":imgdata",0);
        }

        qDebug() << "Inserting into IMAGE-TABLE: " << query.lastQuery();

        query.exec();

        imgID = imageIDFromHash(info->getImageHash());
        qDebug() << "Image found with ID: " << imgID;
    } else {
        qDebug() << "Image already in database, reusing it";
    }
    {
        // No image for this album
        if ( imgID == -1 ) {
            imgID = -2;
        }
        QSqlQuery query;
        query.prepare("INSERT INTO albums ("
                      "albumname, artistname, albuminfo, imageID ) "
                      "VALUES ("
                      ":albumname, :artistname, :albuminfo, :imageID)");
        query.bindValue(":albumname",info->getName());
        query.bindValue(":artistname",info->getArtist());
        // Compress album info here
        QByteArray infoArray;
        infoArray.append(info->getAlbumInfo());
        infoArray = qCompress(infoArray,9);
        query.bindValue(":albuminfo",infoArray);
        query.bindValue(":imageID",imgID);
        qDebug() << "Inserting into ALBUM-TABLE: " << query.lastQuery();
        query.exec();
        emit newStasticReady(updateStatistic());
        emit albumEntered(info->getName());
    }

    // Check if this request was for currently playing title
    // If send ready URL
    if ( info->getName() == mCoverAlbum.getTitle() ) {
        QString url = "image://"  IMAGEPROVIDERNAME  "/albumid/" + QString::number(imgID);
        emit coverAlbumArtReady(QVariant::fromValue(url));
    }

    // Cleanup the memory usage
    info->deleteLater();
}

void ImageDatabase::enterArtistInformation(ArtistInformation  *info)
{
    qDebug() << "Entering artist: " << info->getArtist();
    // Check if artist is already part of the database
    if ( hasArtistArt(info->getArtist()) ) {
        qDebug() << "Artist: " << info->getArtist() << " already part of database, skipping";
        // TODO check if image changed
        info->deleteLater();
        return;
    }

    // Check if Image has image data otherwise skip it
//    if ( !info->getImageData() ) {
//        qDebug() << "Album has no cover image, skipping";
//        return;
//    }

    // Check if image with same hash value is already part of the database
    int imgID = imageIDFromHash(info->getImageHash());

    // Enter Artist into SQL DB
    if (imgID == -1 && info->getImageData() ) {
        QSqlQuery query;
        query.prepare("INSERT INTO images ("
                      "imghash, imgdata ) "
                      "VALUES ("
                      ":imghash, :imgdata)");
        query.bindValue(":imghash", info->getImageHash());
        if(info->getImageData()) {
            query.bindValue(":imgdata",*info->getImageData());
        } else {
            query.bindValue(":imgdata",0);
        }

        qDebug() << "Inserting into IMAGE-TABLE: " << query.lastQuery();

        query.exec();

        imgID = imageIDFromHash(info->getImageHash());
        qDebug() << "Image found with ID: " << imgID;
    } else {
        qDebug() << "Image already in database, reusing it";
    }
    {
        // No image for this artist
        if ( imgID == -1 ) {
            imgID = -2;
        }
        QSqlQuery query;
        query.prepare("INSERT INTO artists ("
                      "name, artistinfo, imageID ) "
                      "VALUES ("
                      ":name, :artistinfo, :imageID)");
        query.bindValue(":name",info->getArtist());
        // Compress artist info here
        QByteArray infoArray;
        infoArray.append(info->getArtistInfo());
        infoArray = qCompress(infoArray,9);
        query.bindValue(":artistinfo",infoArray);
        query.bindValue(":imageID",imgID);

        query.exec();
        emit newStasticReady(updateStatistic());
    }

    // Check if this request was for currently playing title
    // If send ready URL
    if ( info->getArtist() == mCoverArtist.getName() ) {
        QString url = "image://"  IMAGEPROVIDERNAME  "/artistid/" + QString::number(imgID);
        emit coverArtistArtReady(QVariant::fromValue(url));
    }

    // Cleanup the memory usage
    info->deleteLater();
}

int ImageDatabase::imageIDFromHash(QString hashValue)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM images WHERE "
                  "imghash=\"" + hashValue + "\"" );
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();
    while ( query.next() ) {
        int imgID = query.value("id").toInt();
        QString hash = query.value("imghash").toString();
        if ( hash == hashValue ) {
            return imgID;
        }
    }
    return -1;
}

int ImageDatabase::imageIDFromAlbumArtist(QString album, QString artist)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM albums WHERE "
                  "albumname=\"" + album + "\" AND "
                  "artistname=\"" + artist + "\"");
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        QString albumName = query.value("albumname").toString();
        if ( albumName == album) {
            qDebug() << "Found album cover ID: " << query.value("imageID").toInt();
            return query.value("imageID").toInt();
        }
    }
    return -1;
}


int ImageDatabase::imageIDFromAlbum(QString album)
{
    QSqlQuery query;
    album = album.replace('\"',"\\\"");
    query.prepare("SELECT * FROM albums WHERE "
                  "albumname=\"" + album + "\"");
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        QString albumName = query.value("albumname").toString();
        if ( albumName == album) {
            qDebug() << "Found album cover ID: " << query.value("imageID").toInt();
            return query.value("imageID").toInt();
        }
    }
    qDebug() << "Found no image";
    return -1;
}

int ImageDatabase::imageIDFromArtist(QString artist)
{
    QSqlQuery query;
    artist = artist.replace('\"',"\\\"");
    query.prepare("SELECT * FROM artists WHERE "
                  "name=\"" + artist + "\"");
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        QString artistName = query.value("name").toString();
        if ( artistName == artist) {
            qDebug() << "Found artist cover ID: " << query.value("imageID").toInt();
            return query.value("imageID").toInt();
        }
    }
    qDebug() << "Found no image";
    return -1;
}

/**
 * Returns QImage with image from database
 * if image isn't in database check online provider and insert image into db
 * @brief ImageDatabase::getAlbumImage
 * @param album
 * @param artist
 * @return
 */
QImage ImageDatabase::getAlbumImage(QString album, QString artist, bool download)
{
    qDebug() << "get Image for: " << album << artist;
    int artworkID = imageIDFromAlbumArtist(album,artist);
//    if ( artworkID == -1 ) {
//        return QImage();
//    }

    if (artworkID != -1) {
        return getAlbumImage(artworkID);
    }

    if ( download ) {
        qDebug() << "No image found, try downloading";
        MpdAlbum tempAlbum(this,album,artist);
        emit requestAlbumDownload(tempAlbum);
    }

//    if ( download ) {
//        qDebug() << "No image found, try downloading";
//        // Not found an image in database, try retrieving it from internet now
//        // TODO ASYNC PROBLEM
//        // Recheck if album art still doesn't exists
//        LastFMAlbumProvider artProvider("","",this);
//        QEventLoop loop;
//        QObject::connect(&artProvider, SIGNAL(readyRead()), &loop, SLOT(quit()));

//        loop.exec();
//        qDebug() << "Albuminformation downloaded";

//        AlbumInformation *info = artProvider.getLastInformation();
//        enterAlbumInformation(info);
//        QImage img;
//        img.loadFromData(*info->getImageData());
//        return img;
//    }

    return QImage();
}

QImage ImageDatabase::getAlbumImage(QString album,bool download)
{
    int artworkID = imageIDFromAlbum(album);
    if ( artworkID == -1 ) {
        qDebug() << "Returning empty image";
        return QImage();
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM images WHERE "
                  "id=\"" + QString::number(artworkID) + "\"");
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        int id = query.value("id").toInt();
        if ( id == artworkID ) {
            QImage image;
            const QByteArray &imgData = query.value("imgdata").toByteArray();
            if ( image.loadFromData(imgData)) {
                qDebug() << "Image retrieved successfully from database";
                return image;
            }
        }
    }

    // Not found an image in database, try retrieving it from internet now
    // TODO ASYNC PROBLEM
//    LastFMAlbumProvider provider(album,artist);

    return QImage();
}

QImage ImageDatabase::getArtistImage(QString artist,bool download)
{
    int artworkID = imageIDFromArtist(artist);
    if ( artworkID == -1 ) {
        qDebug() << "Returning empty image";
        return QImage();
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM images WHERE "
                  "id=\"" + QString::number(artworkID) + "\"");
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        int id = query.value("id").toInt();
        if ( id == artworkID ) {
            QImage image;
            const QByteArray &imgData = query.value("imgdata").toByteArray();
            if ( image.loadFromData(imgData)) {
                qDebug() << "Image retrieved successfully from database";
                return image;
            }
        }
    }



    return QImage();
}

QImage ImageDatabase::getAlbumImage(int artworkID)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM images WHERE "
                  "id=\"" + QString::number(artworkID) + "\"");
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        int id = query.value("id").toInt();
        if ( id == artworkID ) {
            QImage image;
            const QByteArray &imgData = query.value("imgdata").toByteArray();
            if ( image.loadFromData(imgData)) {
                qDebug() << "Image retrieved successfully from database";
                return image;
            }
        }
    }
    return QImage();
}

QImage ImageDatabase::getArtistImage(int artworkID)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM images WHERE "
                  "id=\"" + QString::number(artworkID) + "\"");
    qDebug() << "Check for image: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        int id = query.value("id").toInt();
        if ( id == artworkID ) {
            QImage image;
            const QByteArray &imgData = query.value("imgdata").toByteArray();
            if ( image.loadFromData(imgData)) {
                qDebug() << "Image retrieved successfully from database";
                return image;
            }
        }
    }
    return QImage();
}

void ImageDatabase::cleanUPBlacklistedAlbums()
{
    QSqlQuery query;
    query.prepare("DELETE FROM albums WHERE "
                  "imageID=\"" + QString::number(-2) + "\"");
    query.exec();
    emit newStasticReady(updateStatistic());
}

void ImageDatabase::cleanupAlbums()
{
    QSqlQuery query;
    query.prepare("DELETE FROM albums ");
    query.exec();
    emit newStasticReady(updateStatistic());
}

void ImageDatabase::cleanupArtists()
{
    QSqlQuery query;
    query.prepare("DELETE FROM artists ");
    query.exec();
    emit newStasticReady(updateStatistic());
}

void ImageDatabase::cleanupDatabase()
{
    cleanupAlbums();
    cleanupArtists();
    QSqlQuery query;
    query.prepare("DELETE FROM images ");
    query.exec();
    emit newStasticReady(updateStatistic());
}


void ImageDatabase::requestCoverImage(MpdAlbum album)
{
    mCoverAlbum = album;
    qDebug() << "get Image for: " << album.getTitle() << album.getArtist();
    int artworkID = imageIDFromAlbumArtist(album.getTitle(),album.getArtist());

    if (artworkID >= 0 ) {
        QString url = "image://"  IMAGEPROVIDERNAME  "/albumid/" + QString::number(artworkID);
        emit coverAlbumArtReady(QVariant::fromValue(url));
        return;
    }
    emit coverAlbumArtReady("");
    emit requestAlbumDownload(album);
}

void ImageDatabase::requestCoverArtistImage(MpdArtist artist)
{
    mCoverArtist = artist;
    qDebug() << "get Image for: " << artist.getName();
    int artworkID = imageIDFromArtist(artist.getName());

    if (artworkID >= 0 ) {
        QString url = "image://"  IMAGEPROVIDERNAME  "/artistid/" + QString::number(artworkID);
        emit coverArtistArtReady(QVariant::fromValue(url));
        return;
    }
    emit coverAlbumArtReady("");
    emit requestArtistDownload(artist);
}

void ImageDatabase::fillDatabase(QList<MpdArtist *> *artistList)
{
    for(int i = 0; i < artistList->size(); i++) {
        MpdArtist *artist = artistList->at(i);
        int imgID = imageIDFromArtist(artist->getName());
        if(imgID == -1 ) {
            qDebug() << "Start downloading info for: " << artist->getName();
            emit requestArtistDownload(MpdArtist(*artist));
        }
    }
}

int ImageDatabase::getAlbumCount()
{
    QSqlQuery query;
    query.prepare("SELECT count(albumname) as albumcount FROM albums");
    query.exec();

    while ( query.next() ) {
        int albumCount = query.value("albumcount").toInt();
        return albumCount;
    }
    return 0;
}

int ImageDatabase::getArtistCount()
{
    QSqlQuery query;
    query.prepare("SELECT count(name) as artistcount FROM artists");
    query.exec();

    while ( query.next() ) {
        int artistCount = query.value("artistcount").toInt();
        return artistCount;
    }
    return 0;
}

int ImageDatabase::getBlacklistCount()
{
    QSqlQuery query;
    query.prepare("SELECT count(albumname) as albumcount FROM albums"
                  " WHERE imageID=\"-2\"");
    query.exec();
    while ( query.next() ) {
       int albumCount = query.value("albumcount").toInt();
       return albumCount;
    }
    return 0;
}

int ImageDatabase::getImageCount()
{
    QSqlQuery query;
    query.prepare("SELECT count(id) as imagecount FROM images");
    query.exec();

    while ( query.next() ) {
        int imageCount = query.value("imagecount").toInt();
        return imageCount;
    }
    return 0;
}

DatabaseStatistic *ImageDatabase::updateStatistic()
{
    // Gather statistic
    mAlbumCount = getAlbumCount();
    mAlbumBlacklistCount = getBlacklistCount();
    mArtistCount = getArtistCount();
    mImageCount = getImageCount();

    return new DatabaseStatistic(mAlbumCount,mAlbumBlacklistCount,mArtistCount,mImageCount,mDownloader->getArtistQueueSize(),mDownloader->getAlbumQueueSize());
}

void ImageDatabase::requestStatisticUpdate()
{
    emit newStasticReady(updateStatistic());
}

QString ImageDatabase::getAlbumWikiInformation(QString album, QString artist)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM albums WHERE "
                  "albumname=\"" + album + "\" AND "
                  "artistname=\"" + artist +"\" ");
    qDebug() << "Check for album wiki information: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        QString retAlbum = query.value("albumname").toString();
        if ( retAlbum == album ) {
            QByteArray compressedBlob = query.value("albuminfo").toByteArray();
            QByteArray uncompressedBlob = qUncompress(compressedBlob);
            QString wikiInfo = QString(uncompressedBlob);
            // Cleanup string for nicer visual representation
            wikiInfo.remove(QRegExp("<[^>]*>"));
            wikiInfo.replace("&quot;","\"");
            return wikiInfo;
        }
    }
    return "";
}

QString ImageDatabase::getArtistBioInformation(QString artist)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM artists WHERE "
                  "name=\"" + artist +"\" ");
    qDebug() << "Check for artist wiki information: " << query.lastQuery();
    query.exec();

    while ( query.next() ) {
        QString retArtist = query.value("name").toString();
        if ( retArtist == artist ) {
            QByteArray compressedBlob = query.value("artistinfo").toByteArray();
            QByteArray uncompressedBlob = qUncompress(compressedBlob);
            QString wikiInfo = QString(uncompressedBlob);
            // Cleanup visual representation
            wikiInfo.remove(QRegExp("<[^>]*>"));
            wikiInfo.replace("&quot;","\"");
            return wikiInfo;
        }
    }
    return "";
}

void ImageDatabase::requestAlbumWikiInformation(QVariant album)
{
    QStringList strings = album.toStringList();
    if(strings.length()==2)
        emit albumWikiInformationReady(getAlbumWikiInformation(strings[0],strings[1]));
}

void ImageDatabase::requestArtistBioInformation(QString artist)
{
    emit artistBioInformationReady(getArtistBioInformation(artist));
}
