#include "mpdalbum.h"

MpdAlbum::MpdAlbum(QObject *parent) :
    QObject(parent)
{
}

MpdAlbum::MpdAlbum(QObject *parent, QString title)
{
    this->title = title;
}

QString MpdAlbum::getTitle()
{
    return title;
}
