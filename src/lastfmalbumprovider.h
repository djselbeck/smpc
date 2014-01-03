#ifndef LASTFMALBUMPROVIDER_H
#define LASTFMALBUMPROVIDER_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QXmlStreamReader>

#include "mpdalbum.h"
#include "mpdartist.h"
#include "common.h"
#include "albuminformation.h"

#define COVERSIZE "small"


class LastFMAlbumProvider : public QObject
{
    Q_OBJECT
public:
    LastFMAlbumProvider(QString albumName,QString artistName);
    LastFMAlbumProvider(MpdArtist &artist);
    ~LastFMAlbumProvider();

    void startDownload();

    QString getImageURL();

signals:
    void ready(AlbumInformation *albumInfo);

private:
    QString mAlbumName;
    QString mArtistName;
    QString mImageURL;
    QString mAlbumInfo;
    QByteArray *mImageData;
    QNetworkAccessManager *mXMLNetAccess;
    QNetworkAccessManager *mAlbumArtAccess;

    void parseAlbum(QXmlStreamReader &xmlReader);
    void parseWikiInformation(QXmlStreamReader &xmlReader);
    void downloadImageData(QUrl imageURL);

private slots:
    void xmlDownloaded(QNetworkReply*);
    void imageDownloaded(QNetworkReply*);
//    void downloadError(QNetworkReply*);

};

#endif // LASTFMURLPROVIDER_H
