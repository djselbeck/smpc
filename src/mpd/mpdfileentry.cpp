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

bool MpdFileEntry::isFile() const
{
    return mType==MpdFileType_File ? true:false;
}

bool MpdFileEntry::isDirectory() const
{
    return mType==MpdFileType_Directory ? true:false;
}

bool MpdFileEntry::isPlaylist() const
{
    return mType==MpdFileType_Playlist ? true:false;
}

QString MpdFileEntry::getName() const
{
    return mName;
}

QString MpdFileEntry::getPrePath() const
{
    return mPrePath;
}

MpdTrack *MpdFileEntry::getTrack() const
{
    if(mTrack!=NULL)
    {
        return mTrack;
    }
    return NULL;
}

QString MpdFileEntry::getTitle() const
{
    if(mTrack!=NULL)
        return getTrack()->getTitle();
    return "";
}
QString MpdFileEntry::getArtist() const
{
    if(mTrack!=NULL)
        return getTrack()->getArtist();
    return "";
}
QString MpdFileEntry::getPath() const
{
    if(mTrack!=NULL)
        return getTrack()->getFileUri();
    return "";
}
QString MpdFileEntry::getAlbum() const
{
    if(mTrack!=NULL)
        return getTrack()->getAlbum();
    return "";
}
QString MpdFileEntry::getLengthFormatted() const
{
    if(mTrack!=NULL)
        return getTrack()->getLengthFormated();
    return "";
}
QString MpdFileEntry::getYear() const
{
    if(mTrack!=NULL)
        return getTrack()->getYear();
    return "";
}

QString MpdFileEntry::getTrackMBID() const
{
    if(mTrack!=NULL)
        return getTrack()->getTrackMBID();
    return "";
}

QString MpdFileEntry::getArtistMBID() const
{
    if(mTrack!=NULL)
        return getTrack()->getArtistMBID();
    return "";
}

QString MpdFileEntry::getAlbumMBID() const
{
    if(mTrack!=NULL)
        return getTrack()->getAlbumMBID();
    return "";
}
QString MpdFileEntry::getSection() const
{
    return (mName=="" ? "" :QString(mName.toUpper()[0]));
}


int MpdFileEntry::getTrackNr() const
{
    if(mTrack!=NULL)
        return getTrack()->getTrackNr();
    return 0;
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

