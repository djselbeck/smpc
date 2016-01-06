#include <localdb/lastfmartistprovider.h>

LastFMArtistProvider::LastFMArtistProvider(QString artistName,QObject *parent) : QObject(parent) {
    mArtistName = artistName;

    mXMLNetAccess = 0;
    mArtistArtAccess = 0;
    mImageData = 0;
    mImageURL = "";
    mArtistInfo = "";
    mLastInformation = 0;
    mDownloadSize = LASTFMDEFAULTSIZE;
}

LastFMArtistProvider::~LastFMArtistProvider() {
    if ( mXMLNetAccess ) {
        disconnect(mXMLNetAccess);
        mXMLNetAccess->deleteLater();
        mXMLNetAccess = 0;
    }
    if ( mArtistArtAccess ) {
        disconnect(mArtistArtAccess);
        mArtistArtAccess->deleteLater();
        mArtistArtAccess = 0;
    }
    if ( mLastInformation ) {
        delete ( mLastInformation );
    }
}


void LastFMArtistProvider::downloadImageData(QUrl imageURL) {
    if(!mArtistArtAccess)
    {
        mArtistArtAccess = new QNetworkAccessManager();
        connect(mArtistArtAccess,SIGNAL(finished(QNetworkReply*)),this,SLOT(imageDownloaded(QNetworkReply*)));
    }
    QNetworkRequest downloadRequest(imageURL);
    mArtistArtAccess->get(downloadRequest);
}

void LastFMArtistProvider::xmlDownloaded(QNetworkReply *reply) {
    QByteArray data = reply->readAll();
    if(data.size() == 0 ) {
        emit failed();
        return;
    }
    qDebug() << "XML received";

    // netaccess not needed anymore
    //    delete(mXMLNetAccess);
    //    mXMLNetAccess = 0;

    //    QString outString(data);

    QXmlStreamReader xmlReader(data);
    bool firstArtistparsed = false;
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

            if ( xmlReader.name() == "artist" && !firstArtistparsed ) {
                parseArtist(xmlReader);
                firstArtistparsed = true;
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
        mLastInformation = new ArtistInformation(mArtistName,mArtistInfo,mImageURL,0,this);
        emit ready(getLastInformation());
    }
}

void LastFMArtistProvider::parseArtist(QXmlStreamReader &xmlReader) {
    // Security check
    if ( ( xmlReader.tokenType() != QXmlStreamReader::StartElement &&
           xmlReader.name() == "artist" )) {
        return;
    }

    bool foundPreferredArtistArt = false;

    while ( !(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "artist") ) {
        if (xmlReader.tokenType() == QXmlStreamReader::Invalid) {
            return;
        }
        if ( xmlReader.tokenType() == QXmlStreamReader::StartElement &&
             xmlReader.name() == "image" && !foundPreferredArtistArt ) {

            // Get image size attribute
            QXmlStreamAttributes attributes = xmlReader.attributes();
            if ( attributes.hasAttribute("size") && attributes.value("size").toString() == mDownloadSize) {
                // Found desired cover size
                xmlReader.readNext();
                if ( xmlReader.tokenType() == QXmlStreamReader::Characters ) {
                    mImageURL = xmlReader.text().toString();
                    foundPreferredArtistArt = true;
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
             xmlReader.name() == "bio" ) {
            parseWikiInformation(xmlReader);
        }


        xmlReader.readNext();
        // Skip similar artists
        if( xmlReader.tokenType() == QXmlStreamReader::StartElement && xmlReader.name() == "similar")
        {
            while ( !(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "similar") )
            {
                xmlReader.readNext();
            }
        }
    }

}

void LastFMArtistProvider::parseWikiInformation(QXmlStreamReader &xmlReader) {
    // Security check
    qDebug() << "Found wiki information";
    if ( ( xmlReader.tokenType() != QXmlStreamReader::StartElement &&
           xmlReader.name() == "bio" )) {
        return;
    }

    while ( !(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "bio") ) {
        if ( xmlReader.tokenType() == QXmlStreamReader::StartElement &&
             xmlReader.name() == "content") {
            while ( !(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "content") )  {
                if (xmlReader.tokenType() == QXmlStreamReader::Invalid) {
                    return;
                }
                if ( xmlReader.tokenType() == QXmlStreamReader::Characters ) {
                    qDebug() << "Reached content block";
                    mArtistInfo.append(xmlReader.text().toString());
                    qDebug() << mArtistInfo;
                }
                xmlReader.readNext();
            }
        }
        xmlReader.readNext();
    }

}

void LastFMArtistProvider::startDownload() {
    // Get last.fm artist info

    QString artistNameClean = mArtistName;
    artistNameClean.replace('#',"%23");
    artistNameClean = artistNameClean.replace('&',"%26");
    artistNameClean = artistNameClean.replace('+',"%2B");
    artistNameClean = artistNameClean.replace(' ',"+");


    QString lastfmXMLURL = QString("http://ws.audioscrobbler.com/2.0/?method=artist.getinfo&api_key=")
            + LASTFMAPIKEY + QString("&artist=") + artistNameClean;
    QUrl requestURL(lastfmXMLURL);
    qDebug() << "Start downloading artist: " << requestURL.toString();

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

    mXMLNetAccess->get(downloadRequest);
    qDebug() << "Netreply get send";
}

void LastFMArtistProvider::imageDownloaded(QNetworkReply *reply) {
    QByteArray *imgData = new QByteArray(reply->readAll());
    if(imgData->size() == 0) {
        emit failed();
        return;
    }
    qDebug() << "Artwork downloaded" << mArtistName;
    if ( mLastInformation ) {
        delete ( mLastInformation );
        mLastInformation = 0;
    }
    mLastInformation = new ArtistInformation(mArtistName,mArtistInfo,mImageURL,imgData,this);
    emit ready(getLastInformation());
}

QString LastFMArtistProvider::getImageURL() {
    return "";
}

ArtistInformation *LastFMArtistProvider::getLastInformation()
{
    return new ArtistInformation(*mLastInformation);
}

void LastFMArtistProvider::setArtist(QString artist)
{
    mArtistName = artist;
}

void LastFMArtistProvider::requestDownload(MpdArtist artist)
{
    mArtistName = artist.getName();
    qDebug() << "Download requested: " << mArtistName;
    if ( mImageData ) {
        delete (mImageData);
        mImageData = 0;
    }
    if ( mImageURL != "" ) {
        mImageURL = "";
    }
    if ( mArtistInfo != "" ) {
        mArtistInfo = "";
    }
    startDownload();
}

void LastFMArtistProvider::setDownloadSize(QString size)
{
    mDownloadSize = size;
}
