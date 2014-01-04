#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QMutex>
#include <QEventLoop>
#include <QQueue>

#include "mpdalbum.h"
#include "lastfmalbumprovider.h"
#include "mpdartist.h"
#include "albuminformation.h"

class ImageDownloader : public QObject
{
    Q_OBJECT
public:

    ImageDownloader();
    ~ImageDownloader();

    QByteArray *getImageBlob();

private:
    QMutex mDownloadCounterMutex;
    int mRunningDownloads;
    QThread *mWorkingThread;
    QQueue<MpdAlbum*> *mQueue;

    LastFMAlbumProvider *mProvider;


signals:
    void albumInformationReady(AlbumInformation *info);

public slots:
    void requestAlbumArt(MpdAlbum *albumObj);
    void albumInformationReceiver(AlbumInformation *info);

};

#endif // IMAGEDOWNLOADER_H
