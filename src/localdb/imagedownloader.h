#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QMutex>
#include <QEventLoop>
#include <QQueue>

#include <mpd/mpdalbum.h>
#include <mpd/mpdartist.h>
#include <localdb/lastfmalbumprovider.h>
#include <localdb/lastfmartistprovider.h>
#include <mpd/mpdartist.h>
#include <localdb/albuminformation.h>
#include <localdb/artistinformation.h>

class ImageDownloader : public QObject
{
    Q_OBJECT
public:

    ImageDownloader();
    ~ImageDownloader();

    int getArtistQueueSize();
    int getAlbumQueueSize();

    void setDownloadSize(QString size);


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

    void clearDownloadQueue();


};

#endif // IMAGEDOWNLOADER_H
