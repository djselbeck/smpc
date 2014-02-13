#ifndef MPDSTATUS_H
#define MPDSTATUS_H

#include <QObject>

class MPDStatus : public QObject
{
    Q_OBJECT
public:
    explicit MPDStatus(QObject *parent = 0);

private:
    QString mInfo;
    QString mTitle;
    QString mArtist;
    QString mAlbum;
    QString mURI;

    int mTrackNR;
    int mPlaylistLength;
    int mSamplerate;
    int mChannelCount;

signals:

public slots:

};

#endif // MPDSTATUS_H
