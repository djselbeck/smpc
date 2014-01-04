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
        if(idList[0] == "album") {
            QImage img = mDB->getAlbumImage(idList[1].toInt());
            qDebug() << "got image";
            size->setHeight(img.height());
            size->setWidth(img.width());
            return img;
        } else {
            QImage img = mDB->getAlbumImage(idList[1]);
            qDebug() << "got image";
            size->setHeight(img.height());
            size->setWidth(img.width());
            return img;
        }
    }
}
