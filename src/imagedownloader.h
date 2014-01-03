#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QByteArray>

#include "mpdalbum.h"
#include "mpdartist.h"

class ImageDownloader : public QObject
{
    Q_OBJECT
public:

    ImageDownloader(MpdAlbum &album);
    ImageDownloader(MpdArtist &artist);

    QByteArray *getImageBlob();

private:
    QString mDownloadURL;


signals:

public slots:

};

#endif // IMAGEDOWNLOADER_H
