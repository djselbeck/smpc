#ifndef DATABASESTATISTIC_H
#define DATABASESTATISTIC_H

#include <QObject>

class DatabaseStatistic : public QObject
{
    Q_OBJECT
public:
    DatabaseStatistic(int albumCount,int albumBlacklistCount, int artistCount, int imageCount, int artistQueueSize, int albumQueueSize, qint64 databaseSize)
    {
        mAlbumCount = albumCount;
        mAlbumBlacklistCount = albumBlacklistCount;
        mArtistCount = artistCount;
        mImageCount = imageCount;
        mArtistQueueSize = artistQueueSize;
        mAlbumQueueSize = albumQueueSize;
        mDatabaseSize = databaseSize;
    }

    Q_INVOKABLE int getAlbumCount()
    {
        return mAlbumCount;
    }
    Q_INVOKABLE int getAlbumBlacklistCount()
    {
        return mAlbumBlacklistCount;
    }
    Q_INVOKABLE int getArtistCount()
    {
        return mArtistCount;
    }
    Q_INVOKABLE int getImageCount()
    {
        return mImageCount;
    }

    Q_INVOKABLE int getArtistQueueSize()
    {
        return mArtistQueueSize;
    }

    Q_INVOKABLE int getAlbumQueueSize()
    {
        return mAlbumQueueSize;
    }


    Q_INVOKABLE qint64 getDatabaseSize()
    {
        return mDatabaseSize;
    }

private:
    int mAlbumCount;
    int mAlbumBlacklistCount;
    int mArtistCount;
    int mImageCount;
    int mArtistQueueSize;
    int mAlbumQueueSize;
    qint64 mDatabaseSize;
};

#endif // DATABASESTATISTIC_H
