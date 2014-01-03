#include "imagedatabase.h"

ImageDatabase::ImageDatabase(QObject *parent) :
    QObject(parent)
{
    mFiller = 0;
    QStringList drivers = QSqlDatabase::drivers();
    if ( !drivers.contains("QSQLITE") ) {
        CommonDebug("No SQLite support");
    } else {
        // Check if database exists already otherwise create on
        QDir dirAccess;
        if ( !dirAccess.mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation))) {
            CommonDebug("Cannot create application data storage folder");
            return;
        }
        QString dbLocation = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/imgDB.sqlite3";
        CommonDebug("Database path: " + dbLocation);
        mDB = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
        mDB->setDatabaseName(dbLocation);
        if ( mDB->open() ) {
            CommonDebug("Database successfully opened");
            // Check if database contains necessary tables
            if ( !mDB->tables().contains("albums") ) {
                QSqlQuery createQuery;
                if (createQuery.exec("CREATE TABLE albums"
                                 "(id integer primary key,"
                                 "albumname text default '',"
                                 "artistname text default '',"
                                 "url text default '',"
                                 "basename text default '',"
                                 "albuminfo text default '',"
                                 "imageID text )") ) {
                    CommonDebug("Albums table created");
                }
            }
            if ( !mDB->tables().contains("albumimages") ) {
                QSqlQuery createQuery;
                if (createQuery.exec("CREATE TABLE albumimages"
                                 "(id integer primary key,"
                                 "url text default '',"
                                 "basename text default '',"
                                 "imghash text default '',"
                                 "imgdata blob)") ) {
                    CommonDebug("Albums table created");
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
                    CommonDebug("artist table created");
                }
            }
        } else {
            CommonDebug("Database open failed");
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
    CommonDebug("got: " + QString(query.size()) + "rows");
    // Search orphaned albums
    while ( query.next() ) {
        QString albumName = query.value("name").toString();
        CommonDebug("Checking: "+ albumName + " for orphan");
        bool foundAlbum = false;
        for (QList<MpdAlbum*>::Iterator it = albums->begin();it != albums->end();it++) {
            if ( (*it)->getTitle() == albumName) {
                foundAlbum = true;
                break;
            }
        }
        if ( !foundAlbum ) {
            albumsToRemove.append(albumName);
            CommonDebug("Album orphaned: " + albumName);
        }
    }
    // remove orphaned albums
    for( int i = 0; i < albumsToRemove.length();i++) {
        QSqlQuery removeQuery;
        if ( removeQuery.exec("DELETE FROM albums where name = \"" +
                         albumsToRemove.at(i) +
                         "\"") ) {
            CommonDebug("Orphaned album: " + albumsToRemove.at(i) + " removed");
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
    CommonDebug("Got album: " + albumName + " with url: " + albumURL + " and " +
                QString::number(imgData->size()) + " bytes image data");
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
    CommonDebug("Fill database");
    if ( mFiller ) {
        return;
    }
    mFiller = new DatabaseFillJob();
    mFiller->startFilling(map);
}

