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
    CommonDebug("Downloading album: " + albumName + " from: " + artistName);
    mCurrentProvider = new LastFMAlbumProvider(albumName,artistName);
    connect(mCurrentProvider,SIGNAL(ready(AlbumInformation*)),this,SLOT(albumFinished(AlbumInformation*)));
    mCurrentProvider->startDownload();
}

void DatabaseFillJob::albumFinished(AlbumInformation *info)
{
    CommonDebug("Album finished: " + info->getName());
    // Check if all artists albums are done, if proceed to next
    CommonDebug("Current artist:" + mCurrentArtistPosition.key()->getName()  +" has: " + QString::number((*mCurrentArtistPosition)->size()) + "albums");
    mCurrentAlbumPosition++;
    if ( mCurrentAlbumPosition == mCurrentArtistPosition.value()->end() ) {
        CommonDebug("All albums from: " + mCurrentArtistPosition.key()->getName() + "processed");
        mCurrentArtistPosition++;
        mCurrentAlbumPosition = (*mCurrentArtistPosition)->begin();
    }
    CommonDebug("deleting old provider");
    delete(mCurrentProvider);
    CommonDebug("deleted old provider");

    QString artistName = mCurrentArtistPosition.key()->getName();
    QString albumName = (*mCurrentAlbumPosition)->getTitle();
    CommonDebug("Downloading album: " + albumName + " from: " + artistName);
    mCurrentProvider = new LastFMAlbumProvider(albumName,artistName);
    connect(mCurrentProvider,SIGNAL(ready(AlbumInformation*)),this,SLOT(albumFinished(AlbumInformation*)));
    mCurrentProvider->startDownload();
}
