#include "mpdartist.h"

MpdArtist::MpdArtist(QObject *parent) :
    QObject(parent)
{
}

MpdArtist::MpdArtist(QObject *parent, QString name) : QObject(parent)
{
    this->mName = name;
}

MpdArtist::MpdArtist(const MpdArtist &copyObject) : QObject(0) {
    this->mName = copyObject.mName;
}

//void MpdArtist::addAlbum(MpdAlbum *album)
//{
//    albums->append(album);
//}

//void MpdArtist::addTrack(MpdTrack *track)
//{
//    tracks->append(track);
//}

//void MpdArtist::addAlbums(QList<MpdAlbum*> *albums)
//{
//   this->albums->append(*albums);
//}

const QString MpdArtist::getName()
{
   return this->mName;
}

//quint32 MpdArtist::albumCount()
//{
//    if(albums!=NULL)
//    {
//        return albums->length();
//    }
//}

//MpdAlbum *MpdArtist::getAlbum(quint32 i)
//{
//    if(albums!=NULL&&i>=0&&i<albums->length())
//    {
//        return albums->at(i);
//    }
//    return NULL;
//}

MpdArtist::~MpdArtist()
{
}
