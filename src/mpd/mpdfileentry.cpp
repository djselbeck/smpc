#include <mpd/mpdfileentry.h>

MpdFileEntry::MpdFileEntry(QObject *parent) :
    QObject(parent)
{
}


MpdFileEntry::MpdFileEntry(QString prepath,QString name, quint8 type ,QObject *parent) : QObject(parent)
{
    this->mName = name;
    this->mType = type;
    this->mPrePath = prepath;
    mTrack = NULL;
}
MpdFileEntry::MpdFileEntry(QString prepath,QString name, quint8 type ,MpdTrack *track,QObject *parent) : QObject(parent)
{
    this->mName = name;
    this->mType = type;
    this->mPrePath = prepath;
    this->mTrack = track;
}

MpdFileEntry::~MpdFileEntry()
{
    if(mTrack!=NULL){
        delete(mTrack);
    }
    mTrack=NULL;
}

const bool MpdFileEntry::isFile()
{
    return mType==MpdFileType_File ? true:false;
}

const bool MpdFileEntry::isDirectory()
{
    return mType==MpdFileType_Directory ? true:false;
}

const bool MpdFileEntry::isPlaylist()
{
    return mType==MpdFileType_Playlist ? true:false;
}

const QString MpdFileEntry::getName()
{
    return mName;
}

const QString MpdFileEntry::getPrePath()
{
    return mPrePath;
}

MpdTrack *MpdFileEntry::getTrack()
{
    if(mTrack!=NULL)
    {
        return mTrack;
    }
    return NULL;
}

QString MpdFileEntry::getTitle()
{
    if(mTrack!=NULL)
        return getTrack()->getTitle();
}
QString MpdFileEntry::getArtist()
{
    if(mTrack!=NULL)
        return getTrack()->getArtist();
}
QString MpdFileEntry::getPath()
{
    if(mTrack!=NULL)
        return getTrack()->getFileUri();
}
QString MpdFileEntry::getAlbum()
{
    if(mTrack!=NULL)
        return getTrack()->getAlbum();
}
QString MpdFileEntry::getLengthFormatted()
{
    if(mTrack!=NULL)
        return getTrack()->getLengthFormated();
}
QString MpdFileEntry::getYear()
{
    if(mTrack!=NULL)
        return getTrack()->getYear();
}

QString MpdFileEntry::getTrackMBID()
{
    if(mTrack!=NULL)
        return getTrack()->getTrackMBID();
}

QString MpdFileEntry::getArtistMBID()
{
    if(mTrack!=NULL)
        return getTrack()->getArtistMBID();
}

QString MpdFileEntry::getAlbumMBID()
{
    if(mTrack!=NULL)
        return getTrack()->getAlbumMBID();
}
QString MpdFileEntry::getSection()
{
    return (mName=="" ? "" :QString(mName.toUpper()[0]));
}


int MpdFileEntry::getTrackNr()
{
    if(mTrack!=NULL)
        return getTrack()->getTrackNr();
}

bool MpdFileEntry::operator< (const MpdFileEntry& other) const
{
    return (mName.compare(other.mName,Qt::CaseInsensitive)<0?1:0);
}

bool MpdFileEntry::operator==(MpdFileEntry & rhs)
{
    return getName()==rhs.getName();
}

bool MpdFileEntry::lessThan(const MpdFileEntry *lhs, const MpdFileEntry* rhs)
{
    return *lhs<*rhs;
}

