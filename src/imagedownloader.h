#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QMutex>
#include <QEventLoop>
#include <QQueue>

#include "mpdalbum.h"
#include "mpdartist.h"
#include "lastfmalbumprovider.h"
#include "lastfmartistprovider.h"
#include "mpdartist.h"
#include "albuminformation.h"
#include "artistinformation.h"

class ImageDownloader : public QObject
{
    Q_OBJECT
public:

    ImageDownloader();
    ~ImageDownloader();

    int getArtistQueueSize();
    int getAlbumQueueSize();


private:
    QThread *mWorkingThread;
    QMutex mDownloadCounterMutex;

    int mRunningAlbumDownloads;
    int mRunningArtistDownloads;

    QQueue<MpdAlbum> *mAlbumQueue;
    QQueue<MpdArtist> *mArtistQueue;

    LastFMAlbumProvider *mAlbumProvider;
    LastFMArtistProvider *mArtistProvider;



signals:
    void albumInformationReady(AlbumInformation *info);
    void artistInformationReady(ArtistInformation *info);

public slots:
    void requestAlbumArt(MpdAlbum albumObj);
    void albumInformationReceiver(AlbumInformation *info);
    void albumDownloadFailure();

    void requestArtistArt(MpdArtist artistObj);
    void artistInformationReceiver(ArtistInformation *info);
    void artistDownloadFailure();

};

#endif // IMAGEDOWNLOADER_H
