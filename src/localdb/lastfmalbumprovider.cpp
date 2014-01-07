#include <localdb/lastfmalbumprovider.h>

LastFMAlbumProvider::LastFMAlbumProvider(QString albumName,QString artistName,QObject *parent) : QObject(parent) {
    mAlbumName = albumName;
    mArtistName = artistName;

    mXMLNetAccess = 0;
    mAlbumArtAccess = 0;
    mImageData = 0;
    mImageURL = "";
    mAlbumInfo = "";
    mLastInformation = 0;
    mDownloadSize = LASTFMDEFAULTSIZE;
}

LastFMAlbumProvider::~LastFMAlbumProvider() {
    if ( mXMLNetAccess ) {
        disconnect(mXMLNetAccess);
        mXMLNetAccess->deleteLater();
        mXMLNetAccess = 0;
    }
    if ( mAlbumArtAccess ) {
        disconnect(mAlbumArtAccess);
        mAlbumArtAccess->deleteLater();
        mAlbumArtAccess = 0;
    }
    if ( mLastInformation ) {
        delete ( mLastInformation );
    }
}


LastFMAlbumProvider::LastFMAlbumProvider(MpdArtist &artist) {

}

void LastFMAlbumProvider::downloadImageData(QUrl imageURL) {
    if(!mAlbumArtAccess)
    {
        mAlbumArtAccess = new QNetworkAccessManager();
        connect(mAlbumArtAccess,SIGNAL(finished(QNetworkReply*)),this,SLOT(imageDownloaded(QNetworkReply*)));
    }
    QNetworkRequest downloadRequest(imageURL);
    mAlbumArtAccess->get(downloadRequest);
}

void LastFMAlbumProvider::xmlDownloaded(QNetworkReply *reply) {
    QByteArray data = reply->readAll();
    if(data.size() == 0) {
        emit failed();
        return;
    }
    qDebug() << "XML received";

    // netaccess not needed anymore
    //    delete(mXMLNetAccess);
    //    mXMLNetAccess = 0;

    //    QString outString(data);

    QXmlStreamReader xmlReader(data);
    while ( !xmlReader.atEnd() ) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if ( token == QXmlStreamReader::StartDocument) {
            // nothing important, skip
            continue;
        }
        else if ( token == QXmlStreamReader::StartElement ) {
            // Found some data
            if ( xmlReader.name() == "lfm" ) {
                continue;
            }

            if ( xmlReader.name() == "album" ) {
                parseAlbum(xmlReader);
            }
        }
    }
    if ( mImageURL != "" ) {
        downloadImageData(mImageURL);
    } else {
        if ( mLastInformation ) {
            delete mLastInformation;
            mLastInformation = 0;
        }
        mLastInformation = new AlbumInformation(mAlbumName,mArtistName,mAlbumInfo,mImageURL,0,this);
        emit ready(getLastInformation());
    }
}

void LastFMAlbumProvider::parseAlbum(QXmlStreamReader &xmlReader) {
    QString albumURL;

    // Security check
    if ( ( xmlReader.tokenType() != QXmlStreamReader::StartElement &&
           xmlReader.name() == "album" )) {
        return;
    }

    bool foundPreferredAlbumArt = false;

    while ( !(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "album") ) {
        if ( xmlReader.tokenType() == QXmlStreamReader::StartElement &&
             xmlReader.name() == "image" && !foundPreferredAlbumArt ) {

            // Get image size attribute
            QXmlStreamAttributes attributes = xmlReader.attributes();
            if ( attributes.hasAttribute("size") && attributes.value("size").toString() == mDownloadSize) {
                // Found desired cover size
                xmlReader.readNext();
                if ( xmlReader.tokenType() == QXmlStreamReader::Characters ) {
                    mImageURL = xmlReader.text().toString();
                    foundPreferredAlbumArt = true;
                }
            }
            else {
                xmlReader.readNext();
                if ( xmlReader.tokenType() == QXmlStreamReader::Characters ) {
                    mImageURL = xmlReader.text().toString();
                }
            }
        }

        if ( xmlReader.tokenType() == QXmlStreamReader::StartElement &&
             xmlReader.name() == "wiki" ) {
            parseWikiInformation(xmlReader);
        }

        xmlReader.readNext();
    }

}

void LastFMAlbumProvider::parseWikiInformation(QXmlStreamReader &xmlReader) {
    // Security check
    if ( ( xmlReader.tokenType() != QXmlStreamReader::StartElement &&
           xmlReader.name() == "wiki" )) {
        return;
    }

    while ( !(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "wiki") ) {
        if ( xmlReader.tokenType() == QXmlStreamReader::StartElement &&
             xmlReader.name() == "content") {
            while ( !(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "content") )  {
                if ( xmlReader.tokenType() == QXmlStreamReader::Characters ) {
                    qDebug() << "Reached content block";
                    mAlbumInfo.append(xmlReader.text().toString());
                    qDebug() << mAlbumInfo;
                }
                xmlReader.readNext();
            }
        }
        xmlReader.readNext();
    }

}

void LastFMAlbumProvider::startDownload() {
    // Get last.fm album info

    QString albumNameClean = mAlbumName;
    albumNameClean.replace('#',"%23");
    albumNameClean = albumNameClean.replace('&',"%26");
    albumNameClean = albumNameClean.replace('+',"%2B");
    albumNameClean = albumNameClean.replace(' ',"+");

    QString artistNameClean = mArtistName;
    artistNameClean.replace('#',"%23");
    artistNameClean = artistNameClean.replace('&',"%26");
    artistNameClean = artistNameClean.replace('+',"%2B");
    artistNameClean = artistNameClean.replace(' ',"+");


    QString lastfmXMLURL = QString("http://ws.audioscrobbler.com/2.0/?method=album.getinfo&api_key=")
            + LASTFMAPIKEY + QString("&artist=") + artistNameClean + QString("&album=") + albumNameClean;
    QUrl requestURL(lastfmXMLURL);
    qDebug() << "Start downloading album: " << requestURL.toString();

    QNetworkRequest downloadRequest(requestURL);
    if( !mXMLNetAccess )
    {
        mXMLNetAccess = new QNetworkAccessManager(this);
        connect(mXMLNetAccess,SIGNAL(finished(QNetworkReply*)),this,SLOT(xmlDownloaded(QNetworkReply*)));
        if( !mXMLNetAccess->networkAccessible()) {
        }
        else {
        }
    }

    QNetworkReply *reply = mXMLNetAccess->get(downloadRequest);
    qDebug() << "Netreply get send";
}

void LastFMAlbumProvider::imageDownloaded(QNetworkReply *reply) {
    QByteArray *imgData = new QByteArray(reply->readAll());
    if(imgData->size() == 0) {
        emit failed();
        return;
    }
    qDebug() << "Artwork downloaded" << mAlbumName;
    if ( mLastInformation ) {
        delete ( mLastInformation );
        mLastInformation = 0;
    }
    mLastInformation = new AlbumInformation(mAlbumName,mArtistName,mAlbumInfo,mImageURL,imgData,this);
    emit ready(getLastInformation());
}

QString LastFMAlbumProvider::getImageURL() {
    return "";
}

AlbumInformation *LastFMAlbumProvider::getLastInformation()
{
    return new AlbumInformation(*mLastInformation);
}

void LastFMAlbumProvider::setArtistAlbum(QString album, QString artist)
{
    mAlbumName = album;
    mArtistName = artist;
}

void LastFMAlbumProvider::requestDownload(MpdAlbum album)
{
    mAlbumName = album.getTitle();
    mArtistName = album.getArtist();
    qDebug() << "Download requested: " << mAlbumName;
    if ( mImageData ) {
        delete (mImageData);
        mImageData = 0;
    }
    if ( mImageURL != "" ) {
        mImageURL = "";
    }
    if ( mAlbumInfo != "" ) {
        mAlbumInfo = "";
    }
//    if ( mAlbumArtAccess ) {
//        mAlbumArtAccess->deleteLater();
//        mAlbumArtAccess = 0;
//    }
//    if ( mXMLNetAccess ) {
//        mXMLNetAccess->deleteLater();
//        mXMLNetAccess = 0;
//    }
    startDownload();
}

void LastFMAlbumProvider::setDownloadSize(QString size)
{
    mDownloadSize = size;
}
