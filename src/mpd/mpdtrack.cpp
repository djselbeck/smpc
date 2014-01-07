#include <mpd/mpdtrack.h>

MpdTrack::MpdTrack(QObject *parent) :
    QObject(parent)
{
  mTitle="";
  mArtist="";
  mAlbum="";
  mLength=0;
  mFileURI="";
  mYear ="";
  mTrackNR = 0;
  mAlbumTracks = 0;
}

MpdTrack::MpdTrack(QObject *parent,QString file, QString title, quint32 length):
        QObject(parent)
{
    this->mLength = length;
    this->mTitle = title;
    this->mFileURI = file;
    mYear = "";
    mTrackNR = 0;
    mAlbumTracks = 0;
}
MpdTrack::MpdTrack(QObject *parent,QString file, QString title, quint32 length,bool playing):
        QObject(parent)
{
    this->mLength = length;
    this->mTitle = title;
    this->mFileURI = file;
    this->mPlaying = playing;
    mYear ="";
    mTrackNR = 0;
    mAlbumTracks = 0;
}

MpdTrack::MpdTrack(QObject *parent, QString file, QString title, QString artist, QString album, quint32 length):
        QObject(parent)
{
    this->mLength = length;
    this->mTitle = title;
    this->mFileURI = file;
    this->mAlbum = album;
    this->mArtist = artist;
    mYear ="";
    mTrackNR = 0;
    mAlbumTracks = 0;
}


QString MpdTrack::getTitle()
{
    return mTitle;
}
QString MpdTrack::getFileUri()
{
    return mFileURI;
}

quint32 MpdTrack::getLength()
{
    return mLength;
}
QString MpdTrack::getAlbum()
{
    return mAlbum;
}

QString MpdTrack::getArtist()
{
    return mArtist;
}

bool MpdTrack::getPlaying()
{
    return mPlaying;
}
QString MpdTrack::getLengthFormated()
{
    QString temp;
    int hours=0,min=0,sec=0;
    hours = mLength/3600;
    if(hours>0)
    {
        min=(mLength-(3600*hours))/60;
    }
    else{
        min=mLength/60;
    }
    sec = mLength-hours*3600-min*60;
    if(hours==0)
    {
        temp=(min<10?"0":"")+QString::number(min)+":"+(sec<10?"0":"")+QString::number(sec);
    }
    else
    {
        temp=(hours<10?"0":"")+QString::number(hours)+":"+(min<10?"0":"")+QString::number(min)+":"+(sec<10?"0":"")+QString::number(sec);
    }
    return temp;
}

QString MpdTrack::getName()
{
  return mFileURI;
}

int MpdTrack::getTrackNr()
{
    return mTrackNR;
}

int MpdTrack::getAlbumTracks()
{
    return mAlbumTracks;
}

QString MpdTrack::getYear()
{
    return mYear;
}

QString MpdTrack::getFileName() {
    QStringList splitted;
    splitted = getFileUri().split('/');
    return (splitted.last() !="" ? splitted.last() : getFileUri());
}

void MpdTrack::setAlbum(QString al)
{
    mAlbum = al;
}
void MpdTrack::setFileUri(QString uri)
{
  this->mFileURI = uri;
}

void MpdTrack::setArtist(QString art)
{
  this->mArtist = art;
}

void MpdTrack::setLength(quint32 length)
{
  this->mLength = length;
}

void MpdTrack::setTitle(QString title)
{
  this->mTitle = title;
}

void MpdTrack::setPlaying(bool playing)
{
    this->mPlaying = playing;
    emit playingchanged();
}

void MpdTrack::setYear(QString year)
{
    this->mYear = year;
}

void MpdTrack::setTrackNr(int nr)
{
    this->mTrackNR = nr;
}

void MpdTrack::setAlbumTracks(int nr)
{
    this->mAlbumTracks = nr;
}
