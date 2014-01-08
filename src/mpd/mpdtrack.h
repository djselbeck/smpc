#ifndef MPDTRACK_H
#define MPDTRACK_H

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <mpd/mpdalbum.h>
#include <mpd/mpdartist.h>

class MpdArtist;
class MpdAlbum;

class MpdTrack : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ getTitle NOTIFY changed )
    Q_PROPERTY(QString uri READ getFileUri NOTIFY changed )
    Q_PROPERTY(QString lengthformatted READ getLengthFormated NOTIFY changed )
    Q_PROPERTY(int length READ getLength NOTIFY changed )
    Q_PROPERTY(QString artist READ getArtist NOTIFY changed )
    Q_PROPERTY(QString album READ getAlbum NOTIFY changed )
    Q_PROPERTY(bool playing READ getPlaying NOTIFY playingchanged )
    Q_PROPERTY(int tracknr READ getTrackNr NOTIFY changed )
    Q_PROPERTY(QString year READ getYear NOTIFY changed )
    Q_PROPERTY(QString filename READ getFileName NOTIFY changed )
public:
    explicit MpdTrack(QObject *parent = 0);
    MpdTrack(QObject *parent,QString file,QString mTitle, quint32 mLength);
    MpdTrack(QObject *parent,QString file,QString mTitle, quint32 mLength,bool mPlaying);
    MpdTrack(QObject *parent,QString file,QString mTitle,QString mArtist, QString mAlbum, quint32 mLength);
    QString getName();

    QString getTitle();
    QString getFileUri();
    quint32 getLength();
    QString getAlbum();
    QString getArtist();
    QString getLengthFormated();
    QString getYear();
    QString getFileName();

    int getTrackNr();
    int getAlbumTracks();
    void setTitle(QString);
    void setFileUri(QString);
    void setLength(quint32 mLength);
    void setAlbum(QString);
    void setArtist(QString);
    void setYear(QString mYear);
    void setTrackNr(int nr);
    void setAlbumTracks(int nr);

    bool getPlaying();
    void setPlaying(bool mPlaying);
private:
    QString mTitle;
    QString mFileURI;
    quint32 mLength;
    QString mArtist;
    QString mAlbum;
    int mTrackNR;
    int mAlbumTracks;
    QString mYear;

    bool mPlaying;

signals:
    void playingchanged();
    void changed();

public slots:

};

#endif // MPDTRACK_H
