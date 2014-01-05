#include "imagedownloader.h"

ImageDownloader::ImageDownloader()
{
    mRunningDownloads = 0;
    mProvider = new LastFMAlbumProvider("","",this);
    mQueue = new QQueue<MpdAlbum>();
//    mWorkingThread = new QThread(this);
//    moveToThread(mWorkingThread);
//    mWorkingThread->start();
    connect(mProvider,SIGNAL(ready(AlbumInformation*)),this,SIGNAL(albumInformationReady(AlbumInformation*)));
    connect(mProvider,SIGNAL(ready(AlbumInformation*)),this,SLOT(albumInformationReceiver(AlbumInformation*)));

//    connect(mWorkingThread, &QThread::finished, mWorkingThread, &QObject::deleteLater);
}

ImageDownloader::~ImageDownloader()
{
    disconnect(mProvider);
    disconnect(this);
    mProvider->deleteLater();
    mProvider = 0;
    delete (mQueue);


//    mWorkingThread->quit();
//    mWorkingThread->wait();
}


void ImageDownloader::requestAlbumArt(MpdAlbum albumObj)
{
    qDebug() << "Art requested for: " << albumObj.getTitle();
    // No running download, start downloading right away
    mDownloadCounterMutex.lock();
    int running = mRunningDownloads;
    mDownloadCounterMutex.unlock();
    if( running == 0 ) {
        qDebug() << "No download running, start now";
        mDownloadCounterMutex.lock();
        mRunningDownloads++;
        mProvider->requestDownload(albumObj);
        mDownloadCounterMutex.unlock();
    } else {
        // Download already running, enqueue
        mQueue->enqueue(albumObj);
    }
}

void ImageDownloader::albumInformationReceiver(AlbumInformation *info)
{
    qDebug() << "Album: " << info->getName() << " downloaded";
    mDownloadCounterMutex.lock();
    mRunningDownloads--;
    int running = mRunningDownloads;
    mDownloadCounterMutex.unlock();
    if ( running == 0 && ( mQueue->size() >0 ) ) {
        mDownloadCounterMutex.lock();
        mRunningDownloads++;
        MpdAlbum albumObj = mQueue->dequeue();
        qDebug() << "Start downloading from queue item: " << albumObj.getTitle();
        mProvider->requestDownload(albumObj);
        mDownloadCounterMutex.unlock();
    } else {
        qDebug() << "finished";
    }
}
