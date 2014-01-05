#include "artistinformation.h"

ArtistInformation::ArtistInformation(QString artist, QString artistInfo, QString url, QByteArray *imgData, QObject *parent) : QObject(parent) {
    mArtistName = artist;
    mArtistInfo = artistInfo;
    mImageURL = url;
    mImageData = imgData;
    if(mImageData) {
        mImgHash = createImageHash(mImageData);
    }
}

ArtistInformation::ArtistInformation(const ArtistInformation &copyObject) {
    mImageURL = copyObject.mImageURL;
    if(copyObject.mImageData) {
        mImageData = new QByteArray(*copyObject.mImageData);
    }
    else {
        mImageData = 0;
    }
    mImgHash = copyObject.mImgHash;
    mArtistInfo = copyObject.mArtistInfo;
    mArtistName = copyObject.mArtistName;
}

QString ArtistInformation::createImageHash(QByteArray *imgData) {
    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(*imgData);
    QString hash(hasher.result().toHex());
    return hash;
}
