#include "mpdplaybackstatus.h"

MPDPlaybackStatus::MPDPlaybackStatus(QObject *parent) :
    QObject(parent)
{
    pPlaylistVersion = 0;
    pID = 0;
    pBitrate = 0;
    pTrackNo = 0;
    pAlbumTrackCount = 0;
    pVolume = 0;
    pTitle = "";
    pAlbum = "";
    pArtist = "";
    pURI = "";
    pPlaybackStatus = MPD_STOP;
    pRepeat = false;
    pShuffle = false;
    pLength = 0;
    pCurrentTime = 0;
    pPlaylistSize = 0;
    pSamplerate = 0;
    pChannelCount = 0;
    pBitDepth = 0;
}

MPDPlaybackStatus::MPDPlaybackStatus(const MPDPlaybackStatus &copyObject, QObject *parent) : QObject(parent)
{
    pPlaylistVersion = copyObject.pPlaylistVersion;
    pID = copyObject.pID;
    pBitrate = copyObject.pBitrate;
    pTrackNo = copyObject.pTrackNo;
    pAlbumTrackCount = copyObject.pAlbumTrackCount;
    pVolume = copyObject.pVolume;
    pTitle = copyObject.pTitle;
    pAlbum = copyObject.pAlbum;
    pArtist = copyObject.pArtist;
    pURI = copyObject.pURI;
    pPlaybackStatus = copyObject.pPlaybackStatus;
    pRepeat = copyObject.pRepeat;
    pShuffle = copyObject.pShuffle;
    pLength = copyObject.pLength;
    pCurrentTime = copyObject.pCurrentTime;
    pPlaylistSize = copyObject.pPlaylistSize;
    pSamplerate = copyObject.pSamplerate;
    pChannelCount = copyObject.pChannelCount;
    pBitDepth = copyObject.pBitDepth;
}

MPDPlaybackStatus::~MPDPlaybackStatus() {

}

quint32 MPDPlaybackStatus::getPlaylistVersion()
{
    return pPlaylistVersion;
}

qint32 MPDPlaybackStatus::getID()
{
    return pID;
}

quint32 MPDPlaybackStatus::getBitrate()
{
    return pBitrate;
}

qint32 MPDPlaybackStatus::getTrackNo()
{
    return pTrackNo;
}

qint32 MPDPlaybackStatus::getAlbumTrackCount()
{
    return pAlbumTrackCount;
}

quint8 MPDPlaybackStatus::getVolume()
{
    return pVolume;
}

QString MPDPlaybackStatus::getTitle()
{
    return pTitle;
}

QString MPDPlaybackStatus::getAlbum()
{
    return pAlbum;
}

QString MPDPlaybackStatus::getArtist()
{
    return pArtist;
}

QString MPDPlaybackStatus::getURI()
{
    return pURI;
}

quint8 MPDPlaybackStatus::getPlaybackStatus()
{
    return pPlaybackStatus;
}

bool MPDPlaybackStatus::getRepeat()
{
    return pRepeat;
}

bool MPDPlaybackStatus::getShuffle()
{
    return pShuffle;
}


quint32 MPDPlaybackStatus::getLength()
{
    return pLength;
}

quint32 MPDPlaybackStatus::getCurrentTime()
{
    return pCurrentTime;
}

quint32 MPDPlaybackStatus::getPlaylistSize()
{
    return pPlaylistSize;
}


quint32 MPDPlaybackStatus::getSamplerate()
{
    return pSamplerate;
}

quint8 MPDPlaybackStatus::getChannelCount()
{
    return pChannelCount;
}

quint8 MPDPlaybackStatus::getBitDepth()
{
    return pBitDepth;
}


void MPDPlaybackStatus::setPlaylistVersion(quint32 playlistVersion)
{
    if (pPlaylistVersion != playlistVersion) {
        pPlaylistVersion = playlistVersion;
        emit playlistVersionChanged();
    }
}

void MPDPlaybackStatus::setID(quint32 id)
{
    if (pID != id) {
        pID = id;
        emit idChanged(pID);
    }
}

void MPDPlaybackStatus::setBitrate(quint32 bitrate)
{
    if (pBitrate != bitrate ) {
        pBitrate = bitrate;
        emit bitrateChanged();
    }
}

void MPDPlaybackStatus::setTrackNo(qint32 trackNo)
{
    if (pTrackNo != trackNo) {
        pTrackNo = trackNo;
        emit trackNoChanged(pTrackNo);
    }
}

void MPDPlaybackStatus::setAlbumTrackCount(qint32 albumTrackCount)
{
    if (pAlbumTrackCount != albumTrackCount) {
        pAlbumTrackCount = albumTrackCount;
        emit albumTrackCountChanged();
    }
}

void MPDPlaybackStatus::setVolume(quint8 volume)
{
    if (pVolume != volume) {
        pVolume = volume;
        emit volumeChanged();
    }
}


void MPDPlaybackStatus::setTitle(QString title)
{
    if (pTitle != title) {
        pTitle = title;
        emit titleChanged();
    }
}

void MPDPlaybackStatus::setAlbum(QString album)
{
    if (pAlbum != album ){
        pAlbum = album;
        emit albumChanged();
    }
}

void MPDPlaybackStatus::setArtist(QString artist)
{
    if (pArtist != artist) {
        pArtist = artist;
        emit artistChanged();
    }
}

void MPDPlaybackStatus::setURI(QString uri)
{
    if (pURI != uri ) {
        pURI = uri;
        emit uriChanged();
    }
}


void MPDPlaybackStatus::setPlaybackStatus(quint8 playbackStatus)
{
    if ( pPlaybackStatus != playbackStatus ) {
        pPlaybackStatus = (MpdPlaybackState)playbackStatus;
        if (pPlaybackStatus == MPD_STOP) {
            clearPlayback();
        }
        emit playbackStatusChanged(pPlaybackStatus);
    }
}

void MPDPlaybackStatus::setRepeat(bool repeat)
{
    if (pRepeat != repeat ) {
        pRepeat = repeat;
        emit repeatChanged();
    }
}

void MPDPlaybackStatus::setShuffle(bool shuffle)
{
    if(pShuffle != shuffle) {
        pShuffle = shuffle;
        emit shuffleChanged();
    }
}


void MPDPlaybackStatus::setLength(quint32 length)
{
    if (pLength != length ) {
        pLength = length;
        emit lengthChanged();
    }
}

void MPDPlaybackStatus::setCurrentTime(quint32 currentTime)
{
    if ( pCurrentTime != currentTime ) {
        pCurrentTime = currentTime;
        emit currentTimeChanged();
    }
}

void MPDPlaybackStatus::setPlaylistSize(quint32 playlistSize)
{
    if ( pPlaylistSize != playlistSize) {
        pPlaylistSize = playlistSize;
        emit playlistSizeChanged();
    }
}

void MPDPlaybackStatus::setSamplerate(quint32 samplerate)
{
    if ( pSamplerate != samplerate ) {
        pSamplerate = samplerate;
        emit samplerateChanged();
    }
}

void MPDPlaybackStatus::setChannelCount(quint8 channelCount)
{
    if ( pChannelCount != channelCount ) {
        pChannelCount = channelCount;
        emit channelCountChanged();
    }
}

void MPDPlaybackStatus::setBitDepth(quint8 bitDepth)
{
    if (pBitDepth != bitDepth) {
        pBitDepth = bitDepth;
        emit bitDepthChanged();
    }
}

void MPDPlaybackStatus::clearPlayback()
{
    /* Clear all playback related items */
    pBitrate = 0;
    emit bitrateChanged();
    pTrackNo = 0;
    emit trackNoChanged(pTrackNo);
    pAlbumTrackCount = 0;
    emit albumTrackCountChanged();
    pTitle = "";
    emit titleChanged();
    pAlbum = "";
    emit albumChanged();
    pArtist = "";
    emit artistChanged();
    pURI = "";
    emit uriChanged();
    pLength = 0;
    emit lengthChanged();
    pCurrentTime = 0;
    emit currentTimeChanged();
    pSamplerate = 0;
    emit samplerateChanged();
    pChannelCount = 0;
    emit channelCountChanged();
    pBitDepth = 0;
    emit bitDepthChanged();
}
