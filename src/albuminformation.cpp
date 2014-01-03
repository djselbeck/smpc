#include "albuminformation.h"

AlbumInformation::AlbumInformation(QString name, QString artist, QString albumInfo, QString url, QByteArray *imgData) : QObject(0) {
    mAlbumName = name;
    mArtistName = artist;
    mAlbumInfo = albumInfo;
    mImageURL = url;
    mImageData = imgData;
    if(mImageData) {
        mImgHash = createImageHash(mImageData);
    }
}

AlbumInformation::AlbumInformation(const AlbumInformation &copyObject) {
    mAlbumName = copyObject.mAlbumName;
    mImageURL = copyObject.mImageURL;
    mImageData = new QByteArray(*copyObject.mImageData);
    mImgHash = copyObject.mImgHash;
}

QString AlbumInformation::createImageHash(QByteArray *imgData) {
    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(*imgData);
    QString hash(hasher.result().toHex());
    return hash;
}
