#include "mpdalbum.h"

MpdAlbum::MpdAlbum(QObject *parent) :
    QObject(parent)
{
}

MpdAlbum::MpdAlbum(QObject *parent, QString title, QString artist)
{
    this->title = title;
    mArtist = artist;
}

MpdAlbum::MpdAlbum(const MpdAlbum &copyObject) {
    this->title = copyObject.title;
}

const QString MpdAlbum::getTitle()
{
    return title;
}

QString MpdAlbum::getArtist() {
    return mArtist;
}
