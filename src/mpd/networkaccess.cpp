#include "networkaccess.h"

/** Constructor for NetworkAccess object. Handles all the network stuff
  */

#define MPD_WHILE_PARSE_LOOP while ((tcpsocket->state()==QTcpSocket::ConnectedState)&&(!response.startsWith("OK"))&&(!response.startsWith("ACK")))

NetworkAccess::NetworkAccess(QObject *parent) :
    QThread(parent)
{
    updateinterval = 1000;
    mPlaylistversion = -1;
    mPlaybackStatus = 0;
    //create socket later used for communication
    tcpsocket = new QTcpSocket(this);
    statusupdater = new QTimer(this);
    connect(tcpsocket,SIGNAL(connected()),this,SLOT(connectedtoServer()));
    connect(tcpsocket,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
    connect(tcpsocket,SIGNAL(disconnected()),this,SLOT(disconnectedfromServer()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(disconnectedfromServer()));
    connect(statusupdater,SIGNAL(timeout()),this,SLOT(interpolateStatus()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorHandle()));

    mIdling = false;
    mIdleCountdown = new QTimer(this);
    mIdleCountdown->setSingleShot(true);
    connect(mIdleCountdown,SIGNAL(timeout()),this,SLOT(goIdle()));
}


/** connects to host and return true if successful, false if not. Takes an string as hostname and int as port */
bool NetworkAccess::connectToHost(QString hostname, quint16 port,QString password)
{
    emit busy();
    mHostname = hostname;
    mPort = port;
    mPassword = password;
    if ( connected() ) {
        tcpsocket->close();
        qDebug() << "Gracefully closed socket";
    }
    tcpsocket->connectToHost(hostname ,port,QIODevice::ReadWrite);

    return true;
}

void NetworkAccess::disconnectFromServer()
{
    emit busy();
    if ( connected() ) {
        sendMPDCommand("close\n");
        tcpsocket->close();
        tcpsocket->waitForDisconnected(5000);
        if ( tcpsocket->state() != QAbstractSocket::UnconnectedState) {
            tcpsocket->abort();
        }
        qDebug() << "Gracefully closed socket";
    }
    emit ready();
}


/** return all albums currently availible from connected MPD as MpdAlbum objects,
  * empty list if not connected or no albums are availible */
void NetworkAccess::getAlbums()
{
    emit busy();
    QList<MpdAlbum*> *albums = new QList<MpdAlbum*>();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        //Start getting list from mpd
        //Send request

        if ( pServerInfo.mpd_cmd_list_group_capabilites ) {
            sendMPDCommand(QString("list album group MUSICBRAINZ_ALBUMID\n"));
        } else {
            sendMPDCommand(QString("list album\n"));
        }

        //Read all albums until OK send from mpd
        QString response ="";
        MpdAlbum *tempalbum;
        QString name;
        QString mbid;
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                /* Remove newline at the end */
                response.chop(1);
                if ( response.startsWith("Album: ") ) {
                    // Append album if name is already set(last album)
                    if ( name != "" ) {
                        tempalbum = new MpdAlbum(NULL,name,"",mbid);
                        tempalbum->moveToThread(mQmlThread);
                        QQmlEngine::setObjectOwnership(tempalbum, QQmlEngine::CppOwnership);
                        albums->append(tempalbum);
                    }
                    name = response.right(response.length() - 7);
                }  else if ( response.startsWith("MUSICBRAINZ_ALBUMID:") ) {
                    mbid = response.right(response.length() - 21);
                }
            }
        }
        /* Append last album also */
        if ( name != "" ) {
            tempalbum = new MpdAlbum(NULL,name,"",mbid);
            tempalbum->moveToThread(mQmlThread);
            QQmlEngine::setObjectOwnership(tempalbum, QQmlEngine::CppOwnership);
            albums->append(tempalbum);
        }
    }

    //Get album tracks
    qSort(albums->begin(),albums->end(),MpdAlbum::lessThan);
    emit ready();
    emit albumsReady((QList<QObject*>*)albums);
}

void NetworkAccess::getArtists()
{
    emit busy();
    emit artistsReady((QList<QObject*>*)getArtists_prv());
    emit ready();

}

QList<MpdArtist*> *NetworkAccess::getArtists_prv()
{
    QList<MpdArtist*> *artists = new QList<MpdArtist*>();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        //Start getting list from mpd
        //Send request
        sendMPDCommand("list artist\n");

        //Read all albums until OK send from mpd
        QString response ="";
        MpdArtist *tempartist=NULL;
        QString name;
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                response.chop(1);
                if (response.startsWith("Artist: "))
                {
                    name = response.right(response.length()-8);
                    tempartist = new MpdArtist(NULL,name);
                    tempartist->moveToThread(mQmlThread);
                    QQmlEngine::setObjectOwnership(tempartist, QQmlEngine::CppOwnership);
                    artists->append(tempartist);
                }

            }
        }
    }
    qSort(artists->begin(),artists->end(),MpdArtist::lessThan);
    return artists;
}


bool NetworkAccess::authenticate(QString password)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {

        QTextStream outstream(tcpsocket);
        outstream.setCodec("UTF-8");
        outstream << "password " << password << "\n";
        outstream.flush();
        tcpsocket->waitForReadyRead(READYREAD);
        //Check Response
        QString response;
        while (tcpsocket->canReadLine())
        {
            response += tcpsocket->readLine();
        }
        QString teststring = response;
        teststring.truncate(2);
        if (teststring==QString("OK"))
        {
            return true;
        }
        else {
            return false;
        }
        return false;
    }
    return false;
}


void NetworkAccess::getArtistsAlbums(QString artist)
{
    emit busy();
    emit(artistAlbumsReady((QList<QObject*>*)getArtistsAlbums_prv(artist)));
    emit ready();
}

QList<MpdAlbum*> *NetworkAccess::getArtistsAlbums_prv(QString artist)
{
    QList<MpdAlbum*> *albums = new QList<MpdAlbum*>();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        //Start getting list from mpd
        //Send request
        artist = artist.replace('\"',"\\\"");
        if ( pServerInfo.mpd_cmd_list_group_capabilites && pServerInfo.mpd_cmd_list_filter_criteria ) {
            sendMPDCommand(QString("list album artist \"")  + artist + "\"" + " group MUSICBRAINZ_ALBUMID\n");
        } else {
            sendMPDCommand(QString("list album \"") + artist + "\"\n");
        }

        //Read all albums until OK send from mpd
        QString response ="";
        MpdAlbum *tempalbum;
        QString name;
        QString mbid;

        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                /* Remove newline at the end */
                response.chop(1);
                if ( response.startsWith("Album: ") ) {
                    // Append album if name is already set(last album)
                    if ( name != "" ) {
                        tempalbum = new MpdAlbum(NULL,name,artist,mbid);
                        tempalbum->moveToThread(mQmlThread);
                        QQmlEngine::setObjectOwnership(tempalbum, QQmlEngine::CppOwnership);
                        albums->append(tempalbum);
                    }
                    name = response.right(response.length() - 7);
                }  else if ( response.startsWith("MUSICBRAINZ_ALBUMID:") ) {
                    mbid = response.right(response.length() - 21);
                }
            }
        }
        /* Append last album also */
        if ( name != "" ) {
            tempalbum = new MpdAlbum(NULL,name,artist,mbid);
            tempalbum->moveToThread(mQmlThread);
            QQmlEngine::setObjectOwnership(tempalbum, QQmlEngine::CppOwnership);
            albums->append(tempalbum);
        }

    }

    //Get album tracks
    qSort(albums->begin(),albums->end(),MpdAlbum::lessThan);
    return albums;

}

void NetworkAccess::getAlbumTracks(QString album)
{
    emit busy();
    emit albumTracksReady((QList<QObject*>*)getAlbumTracks_prv(album));
    emit ready();
}

QList<MpdTrack*>* NetworkAccess::getAlbumTracks_prv(QString album)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        album.replace(QString("\""),QString("\\\""));
        sendMPDCommand(QString("find album \"") + album + "\"\n");
    }
    return parseMPDTracks("");
}

void  NetworkAccess::getAlbumTracks(QString album, QString cartist)
{
    emit busy();
    emit albumTracksReady((QList<QObject*>*)getAlbumTracks_prv(album,cartist));
    emit ready();
}

void  NetworkAccess::getAlbumTracks(QVariant albuminfo)
{
    emit busy();
    QStringList strings = albuminfo.toStringList();
    emit albumTracksReady((QList<QObject*>*)getAlbumTracks_prv(strings[1],strings[0]));
    emit ready();
}

QList<MpdTrack*>*  NetworkAccess::getAlbumTracks_prv(QString album, QString cartist)
{
    if (cartist=="")
    {

        return getAlbumTracks_prv(album);
    }
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        album.replace(QString("\""),QString("\\\""));
        sendMPDCommand(QString("find album \"") + album + "\"\n");
    }
    return parseMPDTracks(cartist);
}

void NetworkAccess::getTracks()
{
    emit busy();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("listallinfo\n");
    }
    emit parseMPDTracks("");
    emit ready();
}



void NetworkAccess::getCurrentPlaylistTracks()
{
    emit busy();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        emit startupdateplaylist();
        sendMPDCommand("playlistinfo\n");
    }
    emit currentPlayListReady((QList<QObject*>*)parseMPDTracks(""));
    emit ready();
}


void NetworkAccess::getPlaylistTracks(QString name)
{
    emit busy();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("listplaylistinfo \"") + name + "\"\n");
    }
    emit savedplaylistTracksReady((QList<QObject*>*)parseMPDTracks(""));
    emit ready();
}


void NetworkAccess::getStatus()
{
    if ( mPlaybackStatus == 0 ) {
        return;
    }
    qDebug() << "::getStatus()";
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        QString response ="";

        QString playlistidstring="-1";
        quint32 playlistversion = 0;
        QString tracknrstring="";

        QString timestring;
        QString elapstr,runstr;

        sendMPDCommand("status\n");

        bool newSong = false;
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                response.chop(1);
                if (response.startsWith("bitrate: ")) {
                    mPlaybackStatus->setBitrate(response.right(response.length()-9).toUInt());
                }
                else if (response.startsWith("time: ")) {
                    timestring = response.right(response.length()-6);
                    elapstr = timestring.split(":").at(0);
                    runstr = timestring.split(":").at(1);
                    mPlaybackStatus->setCurrentTime(elapstr.toInt());
                    mPlaybackStatus->setLength(runstr.toInt());
                }
                else if (response.startsWith("song: ")) {
                    playlistidstring = response.right(response.length()-6);
                    if ( playlistidstring.toUInt() != mPlaybackStatus->getID() ) {
                        newSong = true;
                    }
                    mPlaybackStatus->setID(playlistidstring.toUInt());
                }
                else if (response.startsWith("volume: ")) {
                    mPlaybackStatus->setVolume(response.right(response.length()-8).toUInt());
                }
                else if (response.startsWith("playlist: ")) {
                    playlistversion = response.right(response.length()-10).toUInt();
                    mPlaybackStatus->setPlaylistVersion(playlistversion);
                }
                else if (response.startsWith("playlistlength: ")) {
                    mPlaybackStatus->setPlaylistSize(response.right(response.length()-16).toUInt());
                }
                else if (response.startsWith("state: ")) {
                    {
                        response = response.right(response.length()-7);
                        if (response == "play")
                        {
                            mPlaybackStatus->setPlaybackStatus(MPD_PLAYING);
                            if ( !statusupdater->isActive() ) {
                                statusupdater->start(1000);
                            }
                        }
                        else if (response == "pause") {
                            mPlaybackStatus->setPlaybackStatus(MPD_PAUSE);
                        }
                        else if (response == "stop") {
                            mPlaybackStatus->setPlaybackStatus(MPD_STOP);
                        }
                    }
                }
                else if (response.startsWith("repeat: ")) {
                    {
                        mPlaybackStatus->setRepeat(response.right(response.length()-8) == "1" ? true : false);
                    }
                }
                else if (response.startsWith("random: ")) {
                    {
                        mPlaybackStatus->setShuffle(response.right(response.length()-8) == "1" ? true : false);
                    }
                }
                else if(response.startsWith("audio: ")) {
                    QStringList templist = response.right(response.length()-7).split(":");
                    if(templist.length()==3){
                        mPlaybackStatus->setSamplerate(templist.at(0).toUInt());
                        mPlaybackStatus->setChannelCount(templist.at(2).toUInt());
                        mPlaybackStatus->setBitDepth(templist.at(1).toUInt());
                    }
                }

            }
        }

        if ( newSong ) {
            response = "";
            sendMPDCommand("currentsong\n");
            MPD_WHILE_PARSE_LOOP
            {
                tcpsocket->waitForReadyRead(READYREAD);
                while (tcpsocket->canReadLine())
                {
                    response = QString::fromUtf8(tcpsocket->readLine());
                    response.chop(1);
                    if (response.startsWith("Title: ")) {
                        mPlaybackStatus->setTitle(response.right(response.length()-7));
                    }
                    else if (response.startsWith("Artist: ")) {
                        mPlaybackStatus->setArtist(response.right(response.length()-8));
                    }
                    else if (response.startsWith("Album: ")) {
                        mPlaybackStatus->setAlbum(response.right(response.length()-7));
                    }
                    else if (response.startsWith("file: ")) {
                        mPlaybackStatus->setURI(response.right(response.length()-6));
                    }
                    else if (response.startsWith("Track: "))
                    {
                        tracknrstring = response.right(response.length()-7);
                        //tracknr = tracknrstring.toInt();
                        QStringList tempstrs = tracknrstring.split("/");
                        if(tempstrs.length()==2)
                        {
                            mPlaybackStatus->setTrackNo(tempstrs.first().toUInt());
                            mPlaybackStatus->setAlbumTrackCount(tempstrs.at(1).toUInt());

                        }
                        else if(tempstrs.length()==1)
                        {
                            mPlaybackStatus->setTrackNo(tracknrstring.toUInt());
                        }
                    }
                }
            }
        }

        if(mPlaylistversion!=playlistversion)
        {
            getCurrentPlaylistTracks();
        }
        mPlaylistversion=playlistversion;
        qDebug() << "::getStatus() return";
    }
}


void NetworkAccess::pause()
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        MpdPlaybackState playbackState = getPlaybackState();
        if(playbackState != MPD_STOP) {
            sendMPDCommand("pause\n");
            QString response ="";
            MPD_WHILE_PARSE_LOOP
            {
                tcpsocket->waitForReadyRead(READYREAD);
                while (tcpsocket->canReadLine())
                {
                    response = QString::fromUtf8(tcpsocket->readLine());

                }
            }
            getStatus();
        }
        else {
            playTrackByNumber(getPlaybackID());
        }
    }
    
}

void NetworkAccess::stop()
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("stop\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::enableOutput(int nr)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("enableoutput ") + QString::number(nr) + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
}

void NetworkAccess::disableOutput(int nr)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("disableoutput ") + QString::number(nr) + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
}
void NetworkAccess::updateDB()
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("update\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::next()
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("next\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::previous()
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("previous\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::addAlbumToPlaylist(QString album)
{
    emit busy();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        QList<MpdTrack*> *temptracks = new QList<MpdTrack*>();
        QString response ="";

        temptracks = getAlbumTracks_prv(album);
        //Add Tracks to Playlist
        sendMPDCommand("command_list_begin\n");
        for (int i=0;i<temptracks->length();i++)
        {
            sendMPDCommand(QString("add \"") + temptracks->at(i)->getFileUri() + "\"\n");
        }
        sendMPDCommand("command_list_end\n");
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
    emit ready();
    //   getStatus();
}

void NetworkAccess::addArtistAlbumToPlaylist(QString artist, QString album)
{
    emit busy();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        QList<MpdTrack*> *temptracks = new QList<MpdTrack*>();
        //album.replace(QString("\""),QString("\\\""));
        QString response ="";
        temptracks = getAlbumTracks_prv(album,artist);

        //Add Tracks to Playlist
        sendMPDCommand("command_list_begin\n");
        for (int i=0;i<temptracks->length();i++)
        {
            sendMPDCommand(QString("add \"") + temptracks->at(i)->getFileUri() + "\"\n");
        }
        sendMPDCommand("command_list_end\n");
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
    emit ready();
    //     getStatus();
}



void NetworkAccess::addArtistAlbumToPlaylist(QVariant albuminfo)
{
    QStringList strings = albuminfo.toStringList();
    addArtistAlbumToPlaylist(strings[0],strings[1]);
}

void NetworkAccess::playArtistAlbum(QVariant albuminfo)
{
    clearPlaylist();
    addArtistAlbumToPlaylist(albuminfo);
    playTrackByNumber(0);
    setRandom(false);
    setRepeat(false);
}

void NetworkAccess::playArtistAlbum(QString artist, QString album)
{
    clearPlaylist();
    addArtistAlbumToPlaylist(artist,album);
    playTrackByNumber(0);
    setRandom(false);
    setRepeat(false);
}

void NetworkAccess::playAlbum(QString album)
{
    clearPlaylist();
    addAlbumToPlaylist(album);
    playTrackByNumber(0);
    setRandom(false);
    setRepeat(false);
}


void NetworkAccess::addTrackToPlaylist(QString fileuri)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("add \"") + fileuri + "\"\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
    getStatus();
}

// Format [URI,playlistName]
void NetworkAccess::addTrackToSavedPlaylist(QVariant data)
{
    QStringList inputStrings = data.toStringList();
    if ( inputStrings.size() != 2 ) {
        return;
    }
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("playlistadd \"") + inputStrings.at(1) + "\" \"" + inputStrings.at(0) + "\"\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
    getStatus();
}

// Format [index,playlistName]
void NetworkAccess::removeTrackFromSavedPlaylist(QVariant data)
{
    QStringList inputStrings = data.toStringList();
    if ( inputStrings.size() != 2 ) {
        return;
    }
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("playlistdelete \"") + inputStrings.at(1) + "\" " + inputStrings.at(0) + "\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
    getStatus();
}

void NetworkAccess::playTrackNext(int index)
{
    quint32 currentPosition = getPlaybackID();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        if ( index < currentPosition) {
            sendMPDCommand(QString("move ") + QString::number(index) + " " + QString::number(currentPosition) + "\n");
        } else {
            sendMPDCommand(QString("move ") + QString::number(index) + " " + QString::number(currentPosition + 1) + "\n");
        }
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
    //TODO Workaround
    qDebug() << "Track moved";
//    getStatus();
}

void NetworkAccess::addTrackAfterCurrent(QString fileuri)
{
    quint32 currentPosition = getPlaybackID();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("addid \"") + fileuri + "\" " + QString::number(currentPosition+1) + "\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
    getStatus();
}

//Replace song with uri and plays it back
void NetworkAccess::playFiles(QString fileuri)
{
    clearPlaylist();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("add \"") + fileuri + "\"\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        //Get song id in playlist


        playTrackByNumber(0);
    }
    getStatus();
}


// Append track to playlist and plays it.
void NetworkAccess::playTrack(QString fileuri)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("add \"") + fileuri + "\"\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        //Get song id in playlist

        
        playTrackByNumber(getPlaylistLength() - 1);
    }
    getStatus();
}

void NetworkAccess::playTrackByNumber(int nr)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("play ") + QString::number(nr).toUtf8() + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::deleteTrackByNumer(int nr)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("delete " ) + QString::number(nr).toUtf8() + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::seekPosition(int id, int pos)
{
    qDebug() << "seek: " << id << ":" << pos;
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("seek ") + QString::number(id).toUtf8() + " " +  QString::number(pos).toUtf8() + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::seek(int pos)
{
    seekPosition(getPlaybackID(),pos);
}



void NetworkAccess::setRepeat(bool repeat)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("repeat ") + (repeat ? "1":"0") + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
}

void NetworkAccess::setRandom(bool random)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("random ") + (random ? "1":"0") + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        getStatus();

    }

}

void NetworkAccess::setVolume(int volume)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("setvol ") + QString::number(volume).toUtf8() + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
}

void NetworkAccess::savePlaylist(QString name)
{
    emit ready();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("save \"") + name + "\"\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        if (response.startsWith("OK"))
        {
            emit ready();
            return;
        }
        else {
            emit ready();
            return;
        }

    }
    emit ready();
    return;
}

void NetworkAccess::deletePlaylist(QString name)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("rm \"") + name + "\"\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        if (response.startsWith("OK"))
        {
            return;
        }
        else {
            return;
        }

    }
    return;
}

void NetworkAccess::getSavedPlaylists()
{
    emit busy();
    QStringList *tempplaylists  = new QStringList();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("listplaylists\n");
        QString response ="";
        QString name;

        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                response.chop(1);
                if (response.startsWith("playlist: ")) {
                    name = response.right(response.length()-10);
                    tempplaylists->append(name);
                }
            }
        }

    }
    tempplaylists->sort();
    emit ready();
    emit savedPlaylistsReady(tempplaylists);
}


void NetworkAccess::addPlaylist(QString name)
{
    emit busy();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("load \"") + name + "\"\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
    emit ready();
}

void NetworkAccess::playPlaylist(QString name)
{
    emit busy();
    clearPlaylist();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("load \"") + name + "\"\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
    }
    playTrackByNumber(0);
    emit ready();
}

void NetworkAccess::clearPlaylist()
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("clear\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::disconnectedfromServer()
{
    qDebug() << "Disconnected";
    mPlaylistversion = 0;
    if ( mPlaybackStatus) {
        mPlaybackStatus->clearPlayback();
    }
    if (statusupdater->isActive())
    {
        statusupdater->stop();
    }
    mIdling = false;
    if ( mIdleCountdown->isActive()) {
        mIdleCountdown->stop();
    }
    emit ready();
}

void NetworkAccess::connectedtoServer() {
    qDebug() << "Connected to mpd server";

    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        //Do host authentication
        tcpsocket->waitForReadyRead(READYREAD);
        QString response;
        while (tcpsocket->canReadLine())
        {
            response += tcpsocket->readLine();
        }
        qDebug() << response;
        QString teststring = response;
        teststring.truncate(6);
        if (teststring==QString("OK MPD"))
        {
            QString versionString = response.remove("OK MPD ");
            QStringList versionParts = versionString.split(".");
            if ( versionParts.length() == 3 ) {
                if ( pServerInfo.version.mpdMajor2 != versionParts[1].toUInt()) {
                    qDebug() << "New server version, check capabilities";
                    /* Version has changed, so recheck capabilities */
                    pServerInfo.version.mpdMajor1 = versionParts[0].toUInt();
                    pServerInfo.version.mpdMajor2 = versionParts[1].toUInt();
                    pServerInfo.version.mpdMinor = versionParts[2].toUInt();
                    checkServerCapabilities();
                }
            }
        }

        authenticate(mPassword);
        emit ready();
        emit connectionestablished();
        qDebug() << "Handshake with server done";
    }


    mPlaylistversion = 0;
    if ( mPlaybackStatus) {
        mPlaybackStatus->clearPlayback();
    }
    mIdling = false;

    getStatus();

    statusupdater->start(updateinterval);
}

quint32 NetworkAccess::getPlayListVersion()
{
    quint32 playlistversion = 0;
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("status\n");
        QString response ="";
        QString versionstring;
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                response.chop(1);
                if (response.startsWith("playlist: ")) {
                    versionstring = response.right(response.length()-10);
                    playlistversion = versionstring.toUInt();
                }
            }
        }
    }
    return playlistversion;
}

void NetworkAccess::getDirectory(QString path)
{
    emit busy();
    QList<MpdFileEntry*> *tempfiles = new QList<MpdFileEntry*>();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        path.replace(QString("\""),QString("\\\""));

        sendMPDCommand(QString("lsinfo \"") + path + "\"\n");
        QString response ="";

        MpdTrack *temptrack=NULL;
        MpdFileEntry *tempfile=NULL;
        QString title="";
        QString artist="";
        QString album="";
        QString albumstring="";
        QString datestring = "";
        int nr=0;
        int albumnrs=0;
        QString file="";
        QString filename="";
        QString prepath="";
        QStringList tempsplitter;
        quint32 length=0;

        QString trackMBID;
        QString artistMBID;
        QString albumMBID;

        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                response.chop(1);
                //New file: so new track begins in mpds output
                if (response.startsWith("file: ")) {
                    if (file!=""&&length!=0)
                    {
                        tempsplitter = file.split("/");
                        if (tempsplitter.length()>0)
                        {
                            temptrack = new MpdTrack(NULL,file,title,artist,album,length);
                            temptrack->setYear(datestring);
                            temptrack->setTrackNr(nr);
                            temptrack->setAlbumTracks(albumnrs);
                            temptrack->setAlbumMBID(albumMBID);
                            temptrack->setArtistMBID(artistMBID);
                            temptrack->setTrackMBID(trackMBID);
                            prepath ="";
                            for (int j=0;j<tempsplitter.length()-1;j++)
                            {
                                prepath += tempsplitter.at(j);
                                if (j!=tempsplitter.length()-2)
                                {
                                    prepath += "/";
                                }

                            }
                            tempfile = new MpdFileEntry(prepath,tempsplitter.last(),MpdFileEntry::MpdFileType_File,temptrack,NULL);
                            tempfiles->append(tempfile);
                            temptrack->moveToThread(mQmlThread);
                            tempfile->moveToThread(mQmlThread);
                            QQmlEngine::setObjectOwnership(temptrack, QQmlEngine::CppOwnership);
                            QQmlEngine::setObjectOwnership(tempfile, QQmlEngine::CppOwnership);
                            artistMBID = "";
                            tempsplitter.clear();
                        }
                        artist= "";
                        albumstring="";
                        length=0;
                        album="";
                        title="";
                        filename="";
                        nr=0;
                        datestring="";
                        albumnrs=0;
                        trackMBID = "";
                        artistMBID = "";
                        albumMBID = "";
                    }
                    file = response.right(response.length()-6);
                }
                else if (response.startsWith("Title: ")) {
                    title = response.right(response.length()-7);
                }
                else if (response.startsWith("Artist: ")) {
                    artist = response.right(response.length()-8);
                }
                else if (response.startsWith("Album: ")) {
                    albumstring = response.right(response.length()-7);
                    album = albumstring;
                }
                else if (response.startsWith("Time: ")) {
                    albumstring = response.right(response.length()-6);
                    length = albumstring.toUInt();
                }
                else if (response.startsWith("Date: ")) {
                    datestring = response.right(response.length()-6);
                }
                else if (response.startsWith("Track: ")) {
                    albumstring = response.right(response.length()-7);
                    QStringList tracknrs;
                    tracknrs = albumstring.split('/');
                    if(tracknrs.length()>0)
                    {
                        nr = tracknrs.at(0).toInt();
                        if(tracknrs.length()>1)
                            albumnrs = tracknrs.at(1).toInt();
                    }
                }
                else if (response.startsWith("MUSICBRAINZ_TRACKID: ")) {
                    trackMBID = response.right(response.length()-21);
                }
                else if (response.startsWith("MUSICBRAINZ_ALBUMID: ")) {
                    albumMBID = response.right(response.length()-21);
                }
                else if (response.startsWith("MUSICBRAINZ_ARTISTID: ")) {
                    if ( artistMBID == "" ) {
                        artistMBID = response.right(response.length()-22);
                    }
                }
                //Directory found. WORKS
                else if (response.startsWith("directory: "))
                {
                    filename = response.right(response.length()-11);
                    tempsplitter = filename.split("/");
                    if (tempsplitter.length()>0)
                    {
                        prepath ="";
                        for (int j=0;j<tempsplitter.length()-1;j++)
                        {
                            prepath += tempsplitter.at(j);
                            if (j!=tempsplitter.length()-2)
                            {
                                prepath += "/";
                            }

                        }
                        tempfile = new MpdFileEntry(path,tempsplitter.last(),1,NULL,NULL);
                        tempfiles->append(tempfile);
                        tempfile->moveToThread(mQmlThread);
                        QQmlEngine::setObjectOwnership(tempfile, QQmlEngine::CppOwnership);
                        filename = "";
                        tempsplitter.clear();
                    }

                }
                if (response.startsWith("playlist: "))
                {
                    filename = response.right(response.length()-10);
                    tempsplitter = filename.split("/");
                    if (tempsplitter.length()>0)
                    {
                        prepath ="";
                        for (int j=0;j<tempsplitter.length()-1;j++)
                        {
                            prepath += tempsplitter.at(j);
                            if (j!=tempsplitter.length()-2)
                            {
                                prepath += "/";
                            }

                        }
                        tempfile = new MpdFileEntry(path,tempsplitter.last(),MpdFileEntry::MpdFileType_Playlist,NULL,NULL);
                        tempfiles->append(tempfile);
                        tempfile->moveToThread(mQmlThread);
                        filename = "";
                        tempsplitter.clear();
                    }

                }
            }
        }
        //LAST FILE ADD
        if (file!=""&&title!=""&&length!=0)
        {
            tempsplitter = file.split("/");
            if (tempsplitter.length()>0)
            {
                temptrack = new MpdTrack(NULL,file,title,artist,album,length);
                temptrack->setTrackNr(nr);
                temptrack->setAlbumTracks(albumnrs);
                temptrack->setYear(datestring);
                temptrack->setAlbumMBID(albumMBID);
                temptrack->setArtistMBID(artistMBID);
                temptrack->setTrackMBID(trackMBID);
                prepath ="";
                for (int j=0;j<tempsplitter.length()-1;j++)
                {
                    prepath += tempsplitter.at(j);
                    if (j!=tempsplitter.length()-2)
                    {
                        prepath += "/";
                    }

                }
                qDebug() << "Last album: " << albumstring;
                tempfile = new MpdFileEntry(prepath,tempsplitter.last(),MpdFileEntry::MpdFileType_File,temptrack,NULL);
                tempfiles->append(tempfile);
                temptrack->moveToThread(mQmlThread);
                tempfile->moveToThread(mQmlThread);
                QQmlEngine::setObjectOwnership(temptrack, QQmlEngine::CppOwnership);
                QQmlEngine::setObjectOwnership(tempfile, QQmlEngine::CppOwnership);
                tempsplitter.clear();
            }
        }
    }
    qSort(tempfiles->begin(),tempfiles->end(),MpdFileEntry::lessThan);
    emit ready();
    emit filesReady((QList<QObject*>*)tempfiles);
    //    return tempfiles;
}


void NetworkAccess::setUpdateInterval(int ms)
{
    updateinterval = ms;
    if (statusupdater->isActive())
    {
        statusupdater->stop();
        statusupdater->start(updateinterval);
    }
    if(ms==0){
        statusupdater->stop();
    }
}

bool NetworkAccess::connected()
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState)
    {
        return true;
    } else {
        return false;
    }
}

void NetworkAccess::errorHandle()
{
    tcpsocket->disconnectFromHost();
}

void NetworkAccess::addArtist(QString artist)
{
    QList<MpdAlbum*> *albums = getArtistsAlbums_prv(artist);
    for(int i=0;i<albums->length();i++)
    {

        addArtistAlbumToPlaylist(artist,albums->at(i)->getTitle());
    }
}

void NetworkAccess::playArtist(QString artist)
{
    clearPlaylist();
    addArtist(artist);
    playTrackByNumber(0);
}

void NetworkAccess::setConnectParameters(QString hostname, int port, QString password)
{
    mHostname = hostname;
    mPassword = password;
    mPort = port;
}

void NetworkAccess::connectToHost()
{
    /* Invalidate current playlist */
    mPlaylistversion = 0;
    connectToHost(mHostname,mPort,mPassword);
}

QList<MpdTrack*>* NetworkAccess::parseMPDTracks(QString cartist)
{
    QList<MpdTrack*> *temptracks = new QList<MpdTrack*>();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        QString response ="";

        MpdTrack *temptrack=NULL;
        QString title;
        QString artist;
        QString albumartist;
        QString albumstring;
        QString datestring;
        int nr,albumnrs;
        nr = albumnrs = 0;
        QString file;
        QString trackMBID;
        QString albumMBID;
        QString artistMBID;
        quint32 length=0;
        MPD_WHILE_PARSE_LOOP
        {
            if (!tcpsocket->waitForReadyRead(READYREAD))
            {
            }

            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                // Remove new line
                response.chop(1);
                if (response.startsWith("file: ")) {
                    if (temptrack!=NULL)
                    {
                        // Discard track if artist filter mismatches
                        if (artist==cartist||cartist=="") {
                            temptracks->append(temptrack);
                            artistMBID = "";
                            temptrack->moveToThread(mQmlThread);
                            QQmlEngine::setObjectOwnership(temptrack, QQmlEngine::CppOwnership);
                        } else {
                            delete(temptrack);
                        }
                        temptrack=NULL;
                    }
                    if (temptrack==NULL)
                    {
                        temptrack = new MpdTrack(NULL);
                    }
                    file = response.right(response.length()-6);
                    temptrack->setFileUri(file);
                }
                else if (response.startsWith("Title: ")) {
                    title = response.right(response.length()-7);
                    temptrack->setTitle(title);
                }
                else if (response.startsWith("Artist: ")) {
                    artist = response.right(response.length()-8);
                    temptrack->setArtist(artist);
                }
                else if (response.startsWith("AlbumArtist: ")) {
                    albumartist = response.right(response.length()-13);
                    temptrack->setAlbumArtist(albumartist);
                }
                else if (response.startsWith("Album: ")) {
                    albumstring = response.right(response.length()-7);
                    temptrack->setAlbum(albumstring);
                }

                else if (response.startsWith("Time: ")) {
                    albumstring = response.right(response.length()-6);
                    length = albumstring.toUInt();
                    temptrack->setLength(length);
                }
                else if (response.startsWith("Date: ")) {
                    datestring = response.right(response.length()-6);
                    temptrack->setYear(datestring);
                }
                else if (response.startsWith("MUSICBRAINZ_TRACKID: ")) {
                    trackMBID = response.right(response.length()-21);
                    temptrack->setTrackMBID(trackMBID);
                }
                else if (response.startsWith("MUSICBRAINZ_ALBUMID: ")) {
                    albumMBID = response.right(response.length()-21);
                    temptrack->setAlbumMBID(albumMBID);
                }
                else if (response.startsWith("MUSICBRAINZ_ARTISTID: ")) {
                    if ( artistMBID == "" ) {
                        artistMBID = response.right(response.length()-22);
                        temptrack->setArtistMBID(artistMBID);
                    }
                }
                else if (response.startsWith("Track: ")) {
                    albumstring = response.right(response.length()-7);
                    QStringList tracknrs;
                    tracknrs = albumstring.split('/');
                    if(tracknrs.length()>0)
                    {
                        nr = tracknrs.at(0).toInt();
                        if(tracknrs.length()>1) {
                            albumnrs = tracknrs.at(1).toInt();
                            temptrack->setAlbumTracks(albumnrs);
                        }
                    }
                    temptrack->setTrackNr(nr);
                }
            }

        }
        if (temptrack!=NULL)
        {
            if (artist==cartist||cartist=="") {
                temptrack->setPlaying(false);
                temptracks->append(temptrack);
                temptrack->moveToThread(mQmlThread);
                QQmlEngine::setObjectOwnership(temptrack, QQmlEngine::CppOwnership);
            }
            else {
                delete(temptrack);
                temptrack = NULL;
            }
        }
    }
    return temptracks;
}

void NetworkAccess::exitRequest()
{
    this->disconnectFromServer();
    emit requestExit();
}

void NetworkAccess::getOutputs()
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        emit busy();
        QString response ="";
        QString tempstring;
        QList<MPDOutput*> *outputlist = new QList<MPDOutput*>();
        QString outputname;
        int outputid;
        bool outputenabled;


        sendMPDCommand("outputs\n");
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                response.chop(1);
                if (response.startsWith("outputname: ")) {
                    tempstring = response.right(response.length()-12);
                    outputname = tempstring;
                }
                if (response.startsWith("outputid: ")) {
                    tempstring = response.right(response.length()-10);
                    outputid = tempstring.toInt();
                }
                if (response.startsWith("outputenabled: ")) {
                    tempstring = response.right(response.length()-15);
                    outputenabled = ( tempstring=="1" ? true:false);
                    MPDOutput *tmpOutput = new MPDOutput(outputname,outputenabled,outputid);
                    outputlist->append(tmpOutput);
                    tmpOutput->moveToThread(mQmlThread);
                    QQmlEngine::setObjectOwnership(tmpOutput, QQmlEngine::CppOwnership);
                }


            }
        }
        emit outputsReady((QList<QObject*>*)outputlist);
        emit ready();
    }

}


void NetworkAccess::searchTracks(QVariant request)
{
    emit busy();
    QStringList searchrequest = request.toStringList();
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand(QString("search ") + searchrequest.at(0) + " \"" + searchrequest.at(1) + "\"\n");
    }
    emit searchedTracksReady((QList<QObject*>*)parseMPDTracks(""));
    emit ready();
}

void NetworkAccess::setQmlThread(QThread *thread)
{
    if(thread)
    {
        mQmlThread = thread;
    }
}

void NetworkAccess::getArtistAlbumMap()
{
    emit busy();
    emit artistsAlbumsMapReady(getArtistsAlbumsMap_prv());
    emit ready();
}

QMap<MpdArtist*, QList<MpdAlbum*>* > *NetworkAccess::getArtistsAlbumsMap_prv()
{
    QMap<MpdArtist*, QList<MpdAlbum*>* > *resMap = new QMap<MpdArtist*, QList<MpdAlbum*>* >();
    QList<MpdArtist*> *artists = getArtists_prv();
    for ( int i = 0; i < artists->length(); i++ ) {
        QList<MpdAlbum*> *albums = getArtistsAlbums_prv(artists->at(i)->getName());
        MpdArtist *tmpArtist = artists->at(i);
        (*resMap)[tmpArtist] = albums;
    }
    return resMap;
}


void NetworkAccess::checkServerCapabilities() {
    /* Check server version */
    if ( pServerInfo.version.mpdMajor2 >= 19 ) {
        /* Enable new list command features */
        pServerInfo.mpd_cmd_list_filter_criteria = true;
        pServerInfo.mpd_cmd_list_group_capabilites = true;
        qDebug() << "Enable new list features of version 0.19";
    } else {
        pServerInfo.mpd_cmd_list_filter_criteria = false;
        pServerInfo.mpd_cmd_list_group_capabilites = false;
    }
}


void NetworkAccess::registerPlaybackStatus(MPDPlaybackStatus *playbackStatus)
{
    Q_ASSERT(mPlaybackStatus == 0);
    mPlaybackStatus = playbackStatus;
}

MpdPlaybackState NetworkAccess::getPlaybackState()
{
    MpdPlaybackState playbackState;
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("status\n");
        QString response;
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                response.chop(1);
                if (response.startsWith("state: ")) {
                    {
                        response = response.right(response.length()-7);
                        if (response == "play")
                        {
                            playbackState = MPD_PLAYING;
                        }
                        else if (response == "pause") {
                            playbackState = MPD_PAUSE;
                        }
                        else if (response == "stop") {
                            playbackState = MPD_STOP;
                        }
                    }
                }
            }
        }
    }
    return playbackState;
}

quint32 NetworkAccess::getPlaybackID()
{
    qDebug() << "::getPlaybackID";
    quint32 playbackID;
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("status\n");
        QString response = "";
        MPD_WHILE_PARSE_LOOP
        {
            qDebug() << response;
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                response.chop(1);
                if (response.startsWith("song: ")) {
                    playbackID = response.right(response.length()-6).toUInt();
                }
            }
        }
    }
    qDebug() << "ID: " << playbackID;
    return playbackID;
}

quint32 NetworkAccess::getPlaylistLength()
{
    quint32 playlistLength;
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        sendMPDCommand("status\n");
        QString response = "";
        MPD_WHILE_PARSE_LOOP
        {
            tcpsocket->waitForReadyRead(READYREAD);
            while (tcpsocket->canReadLine())
            {
                response = QString::fromUtf8(tcpsocket->readLine());
                response.chop(1);
                if (response.startsWith("playlistlength: ")) {
                    playlistLength = response.right(response.length()-16).toUInt();
                }
            }
        }
    }
    return playlistLength;
}


void NetworkAccess::interpolateStatus()
{
    if ( mIdling && mPlaybackStatus ) {
        /* Interpolate status here */
        if ( mPlaybackStatus->getPlaybackStatus() == MPD_PLAYING &&
             mPlaybackStatus->getLength() > mPlaybackStatus->getCurrentTime()) {
            mPlaybackStatus->setCurrentTime(mPlaybackStatus->getCurrentTime() + updateinterval/1000);
        }
        if ( mLastSyncTime.elapsed() >= RESYNC_TIME) {
            qDebug() << "resyncing mpd status for time drift";
            getStatus();
            goIdle();
        }
    } else {
        qDebug() << "Not idling, polling status";
        getStatus();
        if ( !mIdleCountdown->isActive() && connected() ) {
            qDebug() << "Idle counter is starting";
            mIdleCountdown->start();
        }
    }
}

void NetworkAccess::goIdle()
{
    if ( mIdleCountdown->isActive()) {
        mIdleCountdown->stop();
    }
    qDebug() << "Start idling";
    /* Start the idling and connect newData signal to slot */
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(newDataInIdle()));
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        QTextStream outstream(tcpsocket);
        outstream.setCodec("UTF-8");
        outstream << "idle mixer player options playlist" << "\n";
        outstream.flush();
    }
    mIdling = true;
    mLastSyncTime.start();
}

void NetworkAccess::cancelIdling()
{
    if ( !mIdling ) {
        return;
    }
    disconnect(tcpsocket,SIGNAL(readyRead()),this,SLOT(newDataInIdle()));
    mIdling = false;
    qDebug() << "Stop idling";
    /* Start the idling and connect newData signal to slot */
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        //Do host authentication
        QTextStream outstream(tcpsocket);
        outstream.setCodec("UTF-8");
        outstream << "noidle" << "\n";
        outstream.flush();
    }
    QString response;
    tcpsocket->waitForReadyRead(READYREAD);
    while (tcpsocket->canReadLine())
    {
        response += tcpsocket->readLine();
    }
    qDebug() << response;

}

void NetworkAccess::newDataInIdle()
{
    QString response;
    while (tcpsocket->canReadLine())
    {
        response += tcpsocket->readLine();
        if ( mIdling && response.contains("changed") ) {
            disconnect(tcpsocket,SIGNAL(readyRead()),this,SLOT(newDataInIdle()));
            qDebug() << "Idle seems to be over";
            mIdling = false;
            getStatus();
        }
        response = "";
    }

}


void NetworkAccess::sendMPDCommand(QString cmd)
{
    if (tcpsocket->state() == QAbstractSocket::ConnectedState) {
        /* It is important to cancel the idle command first
         * otherwise MPD disconnects the client */
        if ( mIdling ) {
            cancelIdling();
        }
        qDebug() << "Executing cmd: " << cmd.replace("/","");
        QTextStream outstream(tcpsocket);
        outstream.setCodec("UTF-8");
        outstream << cmd;
        outstream.flush();
    }
}
