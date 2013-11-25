#ifndef MPDFILEENTRY_H
#define MPDFILEENTRY_H

#include <QObject>
#include "mpdtrack.h"


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
    MpdFileEntry(QString prepath,QString name, quint8 type ,MpdTrack *track,QObject *parent);
    MpdFileEntry(QString prepath,QString name, quint8 type ,QObject *parent);
    ~MpdFileEntry();
    bool isFile();
    bool isDirectory();
    bool isPlaylist();
    QString getName();
    QString getPrePath();
    MpdTrack *getTrack();
    QString getTitle() {
        if(track!=0)
            return getTrack()->getTitle();
    }
    QString getArtist() {
        if(track!=0)
            return getTrack()->getArtist();
    }
    QString getPath() {
        if(track!=0)
            return getTrack()->getFileUri();
    }
    QString getAlbum() {
        if(track!=0)
            return getTrack()->getAlbum();
    }
    QString getLengthFormatted(){
        if(track!=0)
            return getTrack()->getLengthFormated();
    }
    QString getYear() {
        if(track!=0)
            return getTrack()->getYear();
    }
    QString getSection() {
        return (name=="" ? "" :QString(name.toUpper()[0]));
    }

    int getTrackNr() {
        if(track!=0)
            return getTrack()->getTrackNr();
    }
    bool operator< (const MpdFileEntry& other) const { return (name.compare(other.name,Qt::CaseInsensitive)<0?1:0); }
    bool operator==(MpdFileEntry & rhs) {return getName()==rhs.getName();}
    static bool lessThan(const MpdFileEntry *lhs, const MpdFileEntry* rhs) {
        return *lhs<*rhs;
    }

private:
    quint8 type;
    QString name;
    QString prepath;
    MpdTrack *track;

signals:
    void changed();

public slots:

};

#endif // MPDFILEENTRY_H
