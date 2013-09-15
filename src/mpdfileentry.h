#ifndef MPDFILEENTRY_H
#define MPDFILEENTRY_H

#include <QObject>
#include "mpdtrack.h"


class MpdFileEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(QString prepath READ getPrePath)
    Q_PROPERTY(bool isFile READ isFile)
    Q_PROPERTY(bool isDirectory READ isDirectory)
    Q_PROPERTY(bool isPlaylist READ isPlaylist)
    Q_PROPERTY(QString title READ getTitle)
    Q_PROPERTY(QString album READ getAlbum)
    Q_PROPERTY(QString path READ getPath)
    Q_PROPERTY(QString artist READ getArtist)
    Q_PROPERTY(QString length READ getLengthFormatted)
    Q_PROPERTY(int tracknr READ getTrackNr)
    Q_PROPERTY(QString year READ getYear)
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

public slots:

};

#endif // MPDFILEENTRY_H
