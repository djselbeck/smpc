#ifndef ALBUMINFORMATION_H
#define ALBUMINFORMATION_H

#include <QObject>
#include <QCryptographicHash>


class AlbumInformation : public QObject
{
    Q_OBJECT
public:
    explicit AlbumInformation(QString name,QString artist, QString url, QByteArray *imgData);
    AlbumInformation(const AlbumInformation&);

    QString getName() { return mAlbumName; }
    QString getURL() { return mImageURL; }
    QByteArray *getImageData() { return mImageData; }
    QString getImageHash() { return mImgHash; }

private:
    QString mArtistName;
    QString mAlbumName;
    QString mImageURL;
    QString mImgHash;
    QByteArray *mImageData;

    QString createImageHash(QByteArray *imgData);

signals:

public slots:

};

#endif // ALBUMINFORMATION_H
