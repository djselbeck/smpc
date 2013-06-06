#include "mpdartist.h"

MpdArtist::MpdArtist(QObject *parent) :
    QObject(parent)
{
}

MpdArtist::MpdArtist(QObject *parent, QString name)
{
    this->name = name;
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

QString MpdArtist::getName()
{
   return this->name;
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
    CommonDebug("Artist destructed");
}
