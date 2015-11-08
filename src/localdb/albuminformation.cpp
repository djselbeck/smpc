#include <localdb/albuminformation.h>

AlbumInformation::AlbumInformation(QString name, QString artist, QString albumInfo, QString url, QByteArray *imgData, QObject *parent) : QObject(parent) {
    mAlbumName = name;
    mArtistName = artist;
    mAlbumInfo = albumInfo;
    mImageURL = url;
    mImageData = imgData;
    if(mImageData) {
        mImgHash = createImageHash(mImageData);
    }
}

AlbumInformation::AlbumInformation(const AlbumInformation &copyObject) : QObject(0){
    mAlbumName = copyObject.mAlbumName;
    mImageURL = copyObject.mImageURL;
    if(copyObject.mImageData) {
        mImageData = new QByteArray(*copyObject.mImageData);
    }
    else {
        mImageData = 0;
    }
    mImgHash = copyObject.mImgHash;
    mAlbumInfo = copyObject.mAlbumInfo;
    mArtistName = copyObject.mArtistName;
}

AlbumInformation::~AlbumInformation()
{
    // Free memory of image
    if(mImageData)
    {
        delete(mImageData);
    }
}

QString AlbumInformation::createImageHash(QByteArray *imgData) {
    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(*imgData);
    QString hash(hasher.result().toHex());
    return hash;
}
