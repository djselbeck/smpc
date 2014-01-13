#include "qmlimageprovider.h"

QMLImageProvider::QMLImageProvider(ImageDatabase *db) : QQuickImageProvider(QQuickImageProvider::Image,QQuickImageProvider::ForceAsynchronousImageLoading)
{
    mDB = db;
}

QImage QMLImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "Image request: " << id << "requestedSize: " << requestedSize;
    // Format is artist/album
    QStringList idList = id.split('/');
    if(id.length()<2) {
        size->setHeight(0);
        size->setWidth(0);
        return QImage();
    }
    else {
        if(idList[0] == "albumid") {
            QImage *tmpImg = mDB->getAlbumImage(idList[1].toInt());
            QImage retImg;
            if ( requestedSize.isValid()) {
                retImg = tmpImg->scaled(requestedSize, Qt::KeepAspectRatio);
            } else {
                retImg = QImage(*tmpImg);
            }
            delete(tmpImg);
            qDebug() << "got image";
            size->setHeight(retImg.height());
            size->setWidth(retImg.width());
            return retImg;
        } else if (idList[0] == "artistid") {
            QImage *tmpImg = mDB->getArtistImage(idList[1].toInt());
            QImage retImg;
            if ( requestedSize.isValid()) {
                retImg = tmpImg->scaled(requestedSize, Qt::KeepAspectRatio);
            } else {
                retImg = QImage(*tmpImg);
            }
            delete(tmpImg);
            qDebug() << "got image";
            size->setHeight(retImg.height());
            size->setWidth(retImg.width());
            return retImg;
        } else if (idList[0] == "album" && idList.length() == 3 ) {
            QImage *tmpImg = mDB->getAlbumImage(idList[2],idList[1],false);
            QImage retImg;
            if ( tmpImg->size().height() == 0 || tmpImg->size().width()== 0) {
                delete(tmpImg);
                // Try searching for album without artist (samplers,etc)
                tmpImg = mDB->getAlbumImage(idList[2]);
            }
            if ( requestedSize.isValid()) {
                retImg = tmpImg->scaled(requestedSize, Qt::KeepAspectRatio);
            }
            else {
                retImg = QImage(*tmpImg);
            }
            delete(tmpImg);
            qDebug() << "got image";
            size->setHeight(retImg.height());
            size->setWidth(retImg.width());
            return retImg;
        } else if (idList[0] == "artistfromalbum" ) {
            QImage *tmpImg = mDB->getArtistImageForAlbum(idList[1]);
            QImage retImg;
            if ( requestedSize.isValid()) {
                retImg = tmpImg->scaled(requestedSize, Qt::KeepAspectRatio);
            }else {
                retImg = QImage(*tmpImg);
            }
            delete(tmpImg);
            qDebug() << "got image";
            size->setHeight(retImg.height());
            size->setWidth(retImg.width());
            return retImg;
        } else if (idList[0] == "artist" ) {
            QImage *tmpImg = mDB->getArtistImage(idList[1]);
            QImage retImg;
            if ( requestedSize.isValid()) {
                retImg = tmpImg->scaled(requestedSize, Qt::KeepAspectRatio);
            }else {
                retImg = QImage(*tmpImg);
            }
            delete(tmpImg);
            qDebug() << "got image";
            size->setHeight(retImg.height());
            size->setWidth(retImg.width());
            return retImg;
        }
        else {
            QImage *tmpImg = mDB->getAlbumImage(idList[1]);
            QImage retImg;
            if ( requestedSize.isValid()) {
                retImg = tmpImg->scaled(requestedSize, Qt::KeepAspectRatio);
            }else {
                retImg = QImage(*tmpImg);
            }
            delete(tmpImg);
            qDebug() << "got image";
            size->setHeight(retImg.height());
            size->setWidth(retImg.width());
            return retImg;
        }
    }
    size->setHeight(0);
    size->setWidth(0);
    return QImage();
}
