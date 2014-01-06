#include "imagedownloader.h"

ImageDownloader::ImageDownloader()
{
    mRunningAlbumDownloads = 0;
    mRunningArtistDownloads = 0;
    mAlbumProvider = new LastFMAlbumProvider("","",this);
    mArtistProvider = new LastFMArtistProvider("",this);
    mAlbumQueue = new QQueue<MpdAlbum>();
    mArtistQueue = new QQueue<MpdArtist>();

//    mWorkingThread = new QThread(this);
//    moveToThread(mWorkingThread);
//    mWorkingThread->start();

    connect(mAlbumProvider,SIGNAL(ready(AlbumInformation*)),this,SIGNAL(albumInformationReady(AlbumInformation*)));
    connect(mAlbumProvider,SIGNAL(ready(AlbumInformation*)),this,SLOT(albumInformationReceiver(AlbumInformation*)));
    connect(mAlbumProvider,SIGNAL(failed()),this,SLOT(albumDownloadFailure()));

    connect(mArtistProvider,SIGNAL(ready(ArtistInformation*)),this,SIGNAL(artistInformationReady(ArtistInformation*)));
    connect(mArtistProvider,SIGNAL(ready(ArtistInformation*)),this,SLOT(artistInformationReceiver(ArtistInformation*)));
    connect(mArtistProvider,SIGNAL(failed()),this,SLOT(artistDownloadFailure()));

//    connect(mWorkingThread, &QThread::finished, mWorkingThread, &QObject::deleteLater);
}

ImageDownloader::~ImageDownloader()
{
    disconnect(mAlbumProvider);
    disconnect(this);
    mAlbumProvider->deleteLater();
    mAlbumProvider = 0;
    delete (mAlbumQueue);


//    mWorkingThread->quit();
//    mWorkingThread->wait();
}


void ImageDownloader::requestAlbumArt(MpdAlbum albumObj)
{
    qDebug() << "Art requested for: " << albumObj.getTitle();
    // No running download, start downloading right away
    mDownloadCounterMutex.lock();
    int running = mRunningAlbumDownloads;
    mDownloadCounterMutex.unlock();
    if( running == 0 ) {
        qDebug() << "No download running, start now";
        mDownloadCounterMutex.lock();
        mRunningAlbumDownloads++;
        mAlbumProvider->requestDownload(albumObj);
        mDownloadCounterMutex.unlock();
    } else {
        // Download already running, enqueue
        mAlbumQueue->enqueue(albumObj);
    }
}

void ImageDownloader::requestArtistArt(MpdArtist artistObj)
{
    qDebug() << "Art requested for: " << artistObj.getName();
    // No running download, start downloading right away
    mDownloadCounterMutex.lock();
    int running = mRunningArtistDownloads;
    mDownloadCounterMutex.unlock();
    if( running == 0 ) {
        qDebug() << "No download running, start now";
        mDownloadCounterMutex.lock();
        mRunningArtistDownloads++;
        mArtistProvider->requestDownload(artistObj);
        mDownloadCounterMutex.unlock();
    } else {
        // Download already running, enqueue
        mArtistQueue->enqueue(artistObj);
    }
}

void ImageDownloader::albumInformationReceiver(AlbumInformation *info)
{
    qDebug() << "Album: " << info->getName() << " downloaded";
    mDownloadCounterMutex.lock();
    mRunningAlbumDownloads--;
    int running = mRunningAlbumDownloads;
    mDownloadCounterMutex.unlock();
    if ( running == 0 && ( mAlbumQueue->size() >0 ) ) {
        mDownloadCounterMutex.lock();
        mRunningAlbumDownloads++;
        MpdAlbum albumObj = mAlbumQueue->dequeue();
        qDebug() << "Start downloading from queue item: " << albumObj.getTitle();
        mAlbumProvider->requestDownload(albumObj);
        mDownloadCounterMutex.unlock();
    } else {
        qDebug() << "finished";
    }
}

void ImageDownloader::albumDownloadFailure()
{
    qDebug() << "Album download failure";
    mDownloadCounterMutex.lock();
    mRunningAlbumDownloads--;
    int running = mRunningAlbumDownloads;
    mDownloadCounterMutex.unlock();
    if ( running == 0 && ( mAlbumQueue->size() >0 ) ) {
        mDownloadCounterMutex.lock();
        mRunningAlbumDownloads++;
        MpdAlbum albumObj = mAlbumQueue->dequeue();
        qDebug() << "Start downloading from queue item: " << albumObj.getTitle();
        mAlbumProvider->requestDownload(albumObj);
        mDownloadCounterMutex.unlock();
    } else {
        qDebug() << "finished";
    }
}


void ImageDownloader::artistInformationReceiver(ArtistInformation *info)
{
    qDebug() << "Artist: " << info->getArtist() << " downloaded";
    mDownloadCounterMutex.lock();
    mRunningArtistDownloads--;
    int running = mRunningArtistDownloads;
    mDownloadCounterMutex.unlock();
    if ( running == 0 && ( mArtistQueue->size() >0 ) ) {
        mDownloadCounterMutex.lock();
        mRunningArtistDownloads++;
        MpdArtist artistObj = mArtistQueue->dequeue();
        qDebug() << "Start downloading from queue item: " << artistObj.getName();
        mArtistProvider->requestDownload(artistObj);
        mDownloadCounterMutex.unlock();
    } else {
        qDebug() << "finished";
    }
}

void ImageDownloader::artistDownloadFailure()
{
    qDebug() << "Artist download failed";
    mDownloadCounterMutex.lock();
    mRunningArtistDownloads--;
    int running = mRunningArtistDownloads;
    mDownloadCounterMutex.unlock();
    if ( running == 0 && ( mArtistQueue->size() >0 ) ) {
        mDownloadCounterMutex.lock();
        mRunningArtistDownloads++;
        MpdArtist artistObj = mArtistQueue->dequeue();
        qDebug() << "Start downloading from queue item: " << artistObj.getName();
        mArtistProvider->requestDownload(artistObj);
        mDownloadCounterMutex.unlock();
    } else {
        qDebug() << "finished";
    }
}

int ImageDownloader::getArtistQueueSize()
{
    return mArtistQueue->size();
}

int ImageDownloader::getAlbumQueueSize()
{
    return mAlbumQueue->size();
}

void ImageDownloader::setDownloadSize(QString size)
{
    mAlbumProvider->setDownloadSize(size);
    mArtistProvider->setDownloadSize(size);
}
