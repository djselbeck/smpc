#ifndef MPDFILEENTRY_H
#define MPDFILEENTRY_H

#include <QObject>
#include <mpd/mpdtrack.h>


class MpdFileEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY changed )
    Q_PROPERTY(QString prepath READ getPrePath NOTIFY changed )
    Q_PROPERTY(bool isFile READ isFile NOTIFY changed )
    Q_PROPERTY(bool isDirectory READ isDirectory NOTIFY changed )
    Q_PROPERTY(bool isPlaylist READ isPlaylist NOTIFY changed )
    Q_PROPERTY(QString title READ getTitle NOTIFY changed )
    Q_PROPERTY(QString album READ getAlbum NOTIFY changed )
    Q_PROPERTY(QString path READ getPath NOTIFY changed )
    Q_PROPERTY(QString artist READ getArtist NOTIFY changed )
    Q_PROPERTY(QString length READ getLengthFormatted NOTIFY changed )
    Q_PROPERTY(int tracknr READ getTrackNr NOTIFY changed )
    Q_PROPERTY(QString year READ getYear NOTIFY changed )
public:
    enum MpdFileType {MpdFileType_File,MpdFileType_Directory,MpdFileType_Playlist,MpdFileType_NR};
    explicit MpdFileEntry(QObject *parent = 0);
    MpdFileEntry(QString mPrePath,QString mName, quint8 mType ,MpdTrack *mTrack,QObject *parent = 0);
    MpdFileEntry(QString mPrePath,QString mName, quint8 mType ,QObject *parent = 0);
    ~MpdFileEntry();

    bool isFile() const;
    bool isDirectory() const;
    bool isPlaylist() const;

    QString getName() const;
    QString getPrePath() const;
    MpdTrack *getTrack() const;
    QString getTitle() const;
    QString getArtist() const;
    QString getPath() const;
    QString getAlbum() const;
    QString getLengthFormatted() const;
    QString getYear() const;
    QString getSection() const;

    QString getTrackMBID() const;
    QString getAlbumMBID() const;
    QString getArtistMBID() const;

    int getTrackNr() const;
    bool operator< (const MpdFileEntry& other) const;
    bool operator==(MpdFileEntry & rhs);
    static bool lessThan(const MpdFileEntry *lhs, const MpdFileEntry* rhs);

private:
    quint8 mType;
    QString mName;
    QString mPrePath;
    MpdTrack *mTrack;

signals:
    void changed();

public slots:

};

#endif // MPDFILEENTRY_H
