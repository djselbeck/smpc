#include "databasefilljob.h"

DatabaseFillJob::DatabaseFillJob() {
    mCurrentProvider = 0;
    mMap = 0;
    mRunning = false;
}

void DatabaseFillJob::startFilling(QMap<MpdArtist*, QList<MpdAlbum*>* > *map)
{
    // check if no job is currently running
    if ( mRunning ) {
        return;
    }
    if ( !map ) {
        return;
    }
    mMap = map;
    mCurrentArtistPosition = mMap->begin();
    mCurrentAlbumPosition = (*mCurrentArtistPosition)->begin();
    QString artistName = mCurrentArtistPosition.key()->getName();
    QString albumName = (*mCurrentAlbumPosition)->getTitle();
    mCurrentProvider = new LastFMAlbumProvider(albumName,artistName);
    connect(mCurrentProvider,SIGNAL(ready(AlbumInformation*)),this,SLOT(albumFinished(AlbumInformation*)));
    mCurrentProvider->startDownload();
}

void DatabaseFillJob::albumFinished(AlbumInformation *info)
{
    if ( info ) {
        emit albumReady(info);
    }
    mCurrentProvider->deleteLater();
    mCurrentProvider = 0;
    // Check if all artists albums are done, if proceed to next
    mCurrentAlbumPosition++;
    if ( mCurrentAlbumPosition == mCurrentArtistPosition.value()->end() ) {
        mCurrentArtistPosition++;
        if(!(*mCurrentArtistPosition)) {
            return;
        }
        mCurrentAlbumPosition = (*mCurrentArtistPosition)->begin();
    }


    QString artistName = mCurrentArtistPosition.key()->getName();
    QString albumName = (*mCurrentAlbumPosition)->getTitle();
    qDebug() << "Checking album: " << albumName << " from artist: " << artistName << endl;
    mCurrentProvider = new LastFMAlbumProvider(albumName,artistName);
    connect(mCurrentProvider,SIGNAL(ready(AlbumInformation*)),this,SLOT(albumFinished(AlbumInformation*)));
    mCurrentProvider->startDownload();
}
