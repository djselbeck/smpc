#include "qmlimageprovider.h"

QMLImageProvider::QMLImageProvider(ImageDatabase *db) : QQuickImageProvider(QQuickImageProvider::Image,QQuickImageProvider::ForceAsynchronousImageLoading)
{
    mDB = db;
}

QImage QMLImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "Image request: " << id;
    // Format is artist/album
    QStringList idList = id.split('/');
    if(id.length()<2) {
        size->setHeight(0);
        size->setWidth(0);
        return QImage();
    }
    else {
        if(idList[0] == "albumid") {
            QImage img = mDB->getAlbumImage(idList[1].toInt());
            qDebug() << "got image";
            size->setHeight(img.height());
            size->setWidth(img.width());
            return img;
        } else if (idList[0] == "artistid") {
            QImage img = mDB->getArtistImage(idList[1].toInt());
            qDebug() << "got image";
            if ( requestedSize.isValid()) {
                size->setHeight(requestedSize.height());
                size->setWidth(requestedSize.width());
                return img.scaled(requestedSize);
            } else {
                size->setHeight(img.height());
                size->setWidth(img.width());
                return img;
            }
        } else if (idList[0] == "album" && idList.length() == 3 ) {
            QImage img = mDB->getAlbumImage(idList[2],idList[1],true);
            if ( img.size().height() == 0 || img.size().width()== 0) {
                // Try searching for album without artist (samplers,etc)
                img = mDB->getAlbumImage(idList[2]);
            }
            if ( requestedSize.isValid()) {
                size->setHeight(requestedSize.height());
                size->setWidth(requestedSize.width());
                return img.scaled(requestedSize);
            } else {
                size->setHeight(img.height());
                size->setWidth(img.width());
                return img;
            }
        } else if (idList[0] == "artistfromalbum" ) {
            QImage img = mDB->getArtistImageForAlbum(idList[1]);
            if ( requestedSize.isValid()) {
                size->setHeight(requestedSize.height());
                size->setWidth(requestedSize.width());
                return img.scaled(requestedSize);
            } else {
                size->setHeight(img.height());
                size->setWidth(img.width());
                return img;
            }
        } else if (idList[0] == "artist" ) {
            QImage img = mDB->getArtistImage(idList[1]);
            if ( requestedSize.isValid()) {
                size->setHeight(requestedSize.height());
                size->setWidth(requestedSize.width());
                return img.scaled(requestedSize);
            } else {
                size->setHeight(img.height());
                size->setWidth(img.width());
                return img;
            }
        }
        else {
            QImage img = mDB->getAlbumImage(idList[1]);
            if ( requestedSize.isValid()) {
                size->setHeight(requestedSize.height());
                size->setWidth(requestedSize.width());
                return img.scaled(requestedSize);
            } else {
                size->setHeight(img.height());
                size->setWidth(img.width());
                return img;
            }
        }
    }
    size->setHeight(0);
    size->setWidth(0);
    return QImage();
}
