#include "wlitrack.h"

wliTrack::wliTrack( QListWidget * parent , int type ):
    QListWidgetItem(parent)
{
}

wliTrack::wliTrack(QListWidget * parent, int type, MpdTrack *track) : QListWidgetItem(parent,1000)
{
    this->track = track;
    setText(text());
}

wliTrack::~wliTrack()
{
 //   delete(track);
}

QString wliTrack::text()
{
    if(track!=NULL)
    {
      if(track->getTitle()!="") {
        return track->getTitle()+" ("+track->getLengthFormated()+")";
      }
      else {
        return track->getFileUri()+" ("+track->getLengthFormated()+")";
      }
    }
    
    
    return NULL;
}


QString wliTrack::getFileUri(){
    if(track!=NULL)
       return track->getFileUri();
    return NULL;
}

QString wliTrack::getTitle(){
    if(track!=NULL)
        return track->getTitle();
    return NULL;
}

QString wliTrack::getArtist(){
    if(track!=NULL)
        return track->getArtist();
    return NULL;
}

QString wliTrack::getAlbum(){
    if(track!=NULL)
        return track->getAlbum();
    return NULL;
}

QString wliTrack::getInfo(){
    if(track!=NULL)
        return "FIXIT!";
    return NULL;
}

QString wliTrack::getLength()
{
    if(track!=NULL)
        return track->getLengthFormated();
    return NULL;
}

MpdTrack *wliTrack::getTrack()
{
    return track;
}
