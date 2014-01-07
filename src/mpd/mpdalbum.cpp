#include <mpd/mpdalbum.h>

MpdAlbum::MpdAlbum(QObject *parent) :
    QObject(parent)
{
}

MpdAlbum::MpdAlbum(QObject *parent, QString title, QString artist) : QObject(parent)
{
    this->mTitle = title;
    mArtist = artist;
}

MpdAlbum::MpdAlbum(const MpdAlbum &copyObject,QObject *parent) : QObject(parent)  {
    this->mTitle = copyObject.mTitle;
    this->mArtist = copyObject.mArtist;
}

const QString MpdAlbum::getTitle()
{
    return mTitle;
}

QString MpdAlbum::getArtist() {
    return mArtist;
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
QString MpdAlbum::getSection()
{
    return (mTitle=="" ? "" : QString(mTitle.toUpper()[0]) );
}

