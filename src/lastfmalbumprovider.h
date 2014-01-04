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

#define COVERSIZE "extralarge"


class LastFMAlbumProvider : public QObject
{
    Q_OBJECT
public:
    LastFMAlbumProvider(QString albumName,QString artistName,QObject *parent=0);
    LastFMAlbumProvider(MpdArtist &artist);
    ~LastFMAlbumProvider();

    void startDownload();


    void setArtistAlbum(QString album, QString artist);

    QString getImageURL();

    AlbumInformation* getLastInformation();

public slots:
    void requestDownload(MpdAlbum *album);

signals:
    void ready(AlbumInformation *albumInfo);
    void ready();
    void failed();

private:
    QString mAlbumName;
    QString mArtistName;
    QString mImageURL;
    QString mAlbumInfo;
    QByteArray *mImageData;
    QNetworkAccessManager *mXMLNetAccess;
    QNetworkAccessManager *mAlbumArtAccess;
    AlbumInformation *mLastInformation;

    void parseAlbum(QXmlStreamReader &xmlReader);
    void parseWikiInformation(QXmlStreamReader &xmlReader);
    void downloadImageData(QUrl imageURL);

private slots:
    void xmlDownloaded(QNetworkReply*);
    void imageDownloaded(QNetworkReply*);
//    void downloadError(QNetworkReply*);

};

#endif // LASTFMURLPROVIDER_H
