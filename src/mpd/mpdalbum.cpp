#include <mpd/mpdalbum.h>

MpdAlbum::MpdAlbum(QObject *parent) :
    QObject(parent)
{
}

MpdAlbum::MpdAlbum(QObject *parent, QString title, QString artist, QString mbid) : QObject(parent)
{
    mTitle = title;
    mArtist = artist;
    mMBID = mbid;
}

MpdAlbum::MpdAlbum(const MpdAlbum &copyObject,QObject *parent) : QObject(parent)  {
    mTitle = copyObject.mTitle;
    mArtist = copyObject.mArtist;
}

const QString MpdAlbum::getTitle()
{
    return mTitle;
}

const QString MpdAlbum::getArtist() {
    return mArtist;
}

const QString MpdAlbum::getMBID() {
    return mMBID;
}

void MpdAlbum::operator =(MpdAlbum &rhs)
{
    mTitle = rhs.mTitle;
    mArtist = rhs.mArtist;
}

bool MpdAlbum::operator ==(MpdAlbum &rhs) const
{
    return mTitle==rhs.mTitle;
}

bool MpdAlbum::operator <(const MpdAlbum &rhs) const
{
    return (mTitle.compare(rhs.mTitle,Qt::CaseInsensitive)<0?1:0);
}

bool MpdAlbum::lessThan(const MpdAlbum *lhs, const MpdAlbum* rhs)
{
    return *lhs<*rhs;
}


// Return first letter as section
const QString MpdAlbum::getSection()
{
    return (mTitle=="" ? "" : QString(mTitle.toUpper()[0]) );
}

