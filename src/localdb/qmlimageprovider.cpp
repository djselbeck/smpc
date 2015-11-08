#include "qmlimageprovider.h"

QMLImageProvider::QMLImageProvider(ImageDatabase *db) : QQuickImageProvider(QQuickImageProvider::Pixmap,QQuickImageProvider::ForceAsynchronousImageLoading)
{
    mDB = db;
}

QPixmap QMLImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    // Format is artist/album
    QStringList idList = id.split('/');
    if(id.length()<2) {
        size->setHeight(0);
        size->setWidth(0);
        return QPixmap();
    }
    else {
        if(idList[0] == "albumid") {
            QPixmap tmpImg = mDB->getAlbumImage(idList[1].toInt());
            size->setHeight(tmpImg.height());
            size->setWidth(tmpImg.width());
            if ( requestedSize.isValid() && !tmpImg.isNull() ) {
                tmpImg = tmpImg.scaled(requestedSize, Qt::KeepAspectRatio,Qt::SmoothTransformation);
            }
            return tmpImg;
        } else if (idList[0] == "artistid") {
            QPixmap tmpImg = mDB->getArtistImage(idList[1].toInt());
            size->setHeight(tmpImg.height());
            size->setWidth(tmpImg.width());
            if ( requestedSize.isValid() && !tmpImg.isNull() ) {
                tmpImg = tmpImg.scaled(requestedSize, Qt::KeepAspectRatio,Qt::SmoothTransformation);
            }
            return tmpImg;
        } else if (idList[0] == "album" && idList.length() == 3 ) {
            QPixmap tmpImg = mDB->getAlbumImage(idList[2],idList[1],false);
            if ( tmpImg.size().height() == 0 || tmpImg.size().width()== 0) {
                // Try searching for album without artist (samplers,etc)
                tmpImg = mDB->getAlbumImage(idList[2]);
            }
            size->setHeight(tmpImg.height());
            size->setWidth(tmpImg.width());
            if ( requestedSize.isValid() && !tmpImg.isNull() ) {
                tmpImg = tmpImg.scaled(requestedSize, Qt::KeepAspectRatio,Qt::SmoothTransformation);
            }
            return tmpImg;
        } else if (idList[0] == "artistfromalbum" ) {
            QPixmap tmpImg = mDB->getArtistImageForAlbum(idList[1]);
            size->setHeight(tmpImg.height());
            size->setWidth(tmpImg.width());
            if ( requestedSize.isValid() && !tmpImg.isNull() ) {
                tmpImg = tmpImg.scaled(requestedSize, Qt::KeepAspectRatio,Qt::SmoothTransformation);
            }
            return tmpImg;
        } else if (idList[0] == "artist" ) {
            QPixmap tmpImg = mDB->getArtistImage(idList[1]);
            size->setHeight(tmpImg.height());
            size->setWidth(tmpImg.width());
            if ( requestedSize.isValid() && !tmpImg.isNull() ) {
                tmpImg = tmpImg.scaled(requestedSize, Qt::KeepAspectRatio,Qt::SmoothTransformation);
            }
            return tmpImg;
        }
        else {
            QPixmap tmpImg = mDB->getAlbumImage(idList[1]);
            size->setHeight(tmpImg.height());
            size->setWidth(tmpImg.width());
            if ( requestedSize.isValid() && !tmpImg.isNull() ) {
                tmpImg = tmpImg.scaled(requestedSize, Qt::KeepAspectRatio,Qt::SmoothTransformation);
            }
            return tmpImg;
        }
    }
    size->setHeight(0);
    size->setWidth(0);
    return QPixmap();
}
