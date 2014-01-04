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
                                 "url text default '',"
                                 "basename text default '',"
                                 "albuminfo text default '',"
                                 "imageID text )") ) {
                }
            }
            if ( !mDB->tables().contains("albumimages") ) {
                QSqlQuery createQuery;
                if (createQuery.exec("CREATE TABLE albumimages"
                                 "(id integer primary key AUTOINCREMENT,"
                                 "url text default '',"
                                 "basename text default '',"
                                 "imghash text default '',"
                                 "imgdata blob)") ) {
                }
            }
            if ( !mDB->tables().contains("artist") ) {
                QSqlQuery createQuery;
                if (createQuery.exec("CREATE TABLE artists"
                                 "(id integer primary key,"
                                 "name text default '',"
                                 "url text default '',"
                                 "basename text default '',"
                                 "imgdata blob)") ) {
                }
            }
        }
    }
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
    if ( mFiller ) {
        return;
    }
    mFiller = new DatabaseFillJob();
    connect(mFiller,SIGNAL(albumReady(AlbumInformation*)),this,SLOT(enterAlbumInformation(AlbumInformation*)));
    mFiller->startFilling(map);
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

bool ImageDatabase::hasArtistArt(MpdArtist *artist)
{
    return true;
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
    if ( !info->getImageData() ) {
        qDebug() << "Album has no cover image, skipping";
        return;
    }

    // Check if image with same hash value is already part of the database
    int imgID = imageIDFromHash(info->getImageHash());

    // Enter Album into SQL DB
    if (imgID == -1 ) {
        QSqlQuery query;
        query.prepare("INSERT INTO albumimages ("
                      "url, imghash, imgdata ) "
                      "VALUES ("
                      ":url, :imghash, :imgdata)");
        query.bindValue(":url",info->getURL());
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
        QSqlQuery query;
        query.prepare("INSERT INTO albums ("
                      "id, albumname, artistname, url, albuminfo, imageID ) "
                      "VALUES ("
                      ":id, :albumname, :artistname, :url, :albuminfo, :imageID)");
        query.bindValue(":albumname",info->getName());
        query.bindValue(":artistname",info->getArtist());
        query.bindValue(":url",info->getURL());
        query.bindValue(":albuminfo",info->getAlbumInfo());
        query.bindValue(":imageID",imgID);
        qDebug() << "Inserting into ALBUM-TABLE: " << query.lastQuery();

        query.exec();
    }
    // Cleanup the memory usage
    info->deleteLater();
}

int ImageDatabase::imageIDFromHash(QString hashValue)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM albumimages WHERE "
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


/**
 * Returns QImage with image from database
 * if image isn't in database check online provider and insert image into db
 * @brief ImageDatabase::getAlbumImage
 * @param album
 * @param artist
 * @return
 */
QImage ImageDatabase::getAlbumImage(QString album, QString artist)
{
    int artworkID = imageIDFromAlbumArtist(album,artist);
    if ( artworkID == -1 ) {
        return QImage();
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM albumimages WHERE "
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
