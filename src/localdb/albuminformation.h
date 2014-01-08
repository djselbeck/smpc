#ifndef ALBUMINFORMATION_H
#define ALBUMINFORMATION_H

#include <QObject>
#include <QCryptographicHash>


class AlbumInformation : public QObject
{
    Q_OBJECT
public:
    explicit AlbumInformation(QString name,QString artist, QString albumInfo, QString url, QByteArray *imgData,QObject *parent= 0);
    AlbumInformation(const AlbumInformation&);
    ~AlbumInformation();

    QString getName() { return mAlbumName; }
    QString getArtist() { return mArtistName; }
    QString getURL() { return mImageURL; }
    QString getAlbumInfo() { return mAlbumInfo; }
    QByteArray *getImageData() { return mImageData; }
    QString getImageHash() { return mImgHash; }

private:
    QString mArtistName;
    QString mAlbumName;
    QString mAlbumInfo;
    QString mImageURL;
    QString mImgHash;
    QByteArray *mImageData;

    QString createImageHash(QByteArray *imgData);

signals:

public slots:

};

#endif // ALBUMINFORMATION_H
