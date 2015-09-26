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
  mPlaying = false;
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
    mPlaying = false;
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
    mPlaying = false;
}


const QString MpdTrack::getTitle()
{
    return mTitle;
}
const QString MpdTrack::getFileUri()
{
    return mFileURI;
}

const quint32 MpdTrack::getLength()
{
    return mLength;
}
const QString MpdTrack::getAlbum()
{
    return mAlbum;
}

const QString MpdTrack::getArtist()
{
    return mArtist;
}

const QString MpdTrack::getAlbumArtist()
{
    return mAlbumArtist;
}

const bool MpdTrack::getPlaying()
{
    return mPlaying;
}
const QString MpdTrack::getLengthFormated()
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

const QString MpdTrack::getName()
{
  return mFileURI;
}

const int MpdTrack::getTrackNr()
{
    return mTrackNR;
}

const int MpdTrack::getAlbumTracks()
{
    return mAlbumTracks;
}

const QString MpdTrack::getYear()
{
    return mYear;
}

const QString MpdTrack::getFileName() {
    QStringList splitted;
    splitted = getFileUri().split('/');
    return (splitted.last() !="" ? splitted.last() : getFileUri());
}

const QString MpdTrack::getTrackMBID() {
    return mTrackMBID;
}

const QString MpdTrack::getAlbumMBID() {
    return mAlbumMBID;
}

const QString MpdTrack::getArtistMBID() {
    return mArtistMBID;
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

void MpdTrack::setAlbumArtist(QString artist)
{
    mAlbumArtist = artist;
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

void MpdTrack::setTrackMBID(QString mbid) {
    mTrackMBID = mbid;
}

void MpdTrack::setAlbumMBID(QString mbid) {
    mAlbumMBID = mbid;
}

void MpdTrack::setArtistMBID(QString mbid) {
    mArtistMBID = mbid;
}
