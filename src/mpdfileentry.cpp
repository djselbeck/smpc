#include "mpdfileentry.h"

MpdFileEntry::MpdFileEntry(QObject *parent) :
    QObject(parent)
{
}


MpdFileEntry::MpdFileEntry(QString prepath,QString name, quint8 type ,QObject *parent) : QObject(parent)
{
    this->name = name;
    this->type = type;
    this->prepath = prepath;
    track = NULL;
}
MpdFileEntry::MpdFileEntry(QString prepath,QString name, quint8 type ,MpdTrack *track,QObject *parent) : QObject(parent)
{
    this->name = name;
    this->type = type;
    this->prepath = prepath;
    this->track = track;
}

MpdFileEntry::~MpdFileEntry()
{
    if(track!=NULL) {delete(track);}
    track=NULL;
}

bool MpdFileEntry::isFile()
{
    return type==MpdFileType_File ? true:false;
}

bool MpdFileEntry::isDirectory()
{
    return type==MpdFileType_Directory ? true:false;
}

bool MpdFileEntry::isPlaylist()
{
    return type==MpdFileType_Playlist ? true:false;
}

QString MpdFileEntry::getName()
{
    return name;
}

QString MpdFileEntry::getPrePath()
{
    return prepath;
}

MpdTrack *MpdFileEntry::getTrack()
{
    if(track!=NULL)
    {
        return track;
    }
    return NULL;
}

