#include "networkaccess.h"

/** Constructor for NetworkAccess object. Handles all the network stuff
  */

#define MPD_WHILE_PARSE_LOOP while (Q_LIKELY((mTCPSocket->state()==QTcpSocket::ConnectedState)&&(!response.startsWith("OK"))&&(!response.startsWith("ACK"))))

NetworkAccess::NetworkAccess(QObject *parent) :
    QThread(parent)
{
    mHostname = "";
    mPort = 6600;
    mPassword = "";

    mStatusInterval = 1000;
    mPlaylistversion = -1;
    mPlaybackStatus = new MPDPlaybackStatus();
    //create socket later used for communication
    mTCPSocket = 0;
    mStatusTimer = new QTimer(this);
    mQMLThread = 0;


    // Status updating/interpolation
    connect(mStatusTimer,SIGNAL(timeout()),this,SLOT(interpolateStatus()));

    // MPD idle stuff
    mIdling = false;
    // Timer used to countdown to idle mode. This ensures that the client not misses anything importang.
    mIdleCountdown = new QTimer(this);
    mIdleCountdown->setSingleShot(true);
    connect(mIdleCountdown,SIGNAL(timeout()),this,SLOT(goIdle()));

    /* Reset server capabilities */
    pServerInfo.mpd_cmd_list_filter_criteria = false;
    pServerInfo.mpd_cmd_list_group_capabilites = false;
    pServerInfo.mpd_cmd_idle = false;

    mTimeoutTimer = 0;
}

NetworkAccess::~NetworkAccess()
{
    if(connected() ) {
        // Try to disconnect here
        disconnectFromServer();
    }
    delete(mPlaybackStatus);
}

/** connects to host and return true if successful, false if not. Takes an string as hostname and int as port */
void NetworkAccess::connectToHost(QString hostname, quint16 port,QString password)
{
    emit busy();
    mHostname = hostname;
    mPort = port;
    mPassword = password;
    /* Check if the client is currently connected to a server, if yes disconnect */

    disconnectFromServer();

    Q_ASSERT(!mTCPSocket);

    if ( !mTCPSocket ) {
        qDebug() << "Created new socket";
        mTCPSocket = new QTcpSocket(this);

        // TCP signal handling
        connect(mTCPSocket,SIGNAL(connected()),this,SLOT(onServerConnected()));
        connect(mTCPSocket,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
        connect(mTCPSocket,SIGNAL(disconnected()),this,SLOT(onServerDisconnected()));
        connect(mTCPSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onConnectionError()));
        connect(mTCPSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(onConnectionStateChanged(QAbstractSocket::SocketState)));
    }

    mIdling = false;

    // Initiate TCP connection here
    qDebug() << "Connecting to: " << hostname << ":" << port;
    mTCPSocket->connectToHost(hostname ,port,QIODevice::ReadWrite);
}

void NetworkAccess::disconnectFromServer()
{
    qDebug() << "Disconnect requested";

    emit busy();
    // Check if the socket is really connected before trying to close the connection.
    if ( connected() && mTCPSocket ) {
        // Send connection termination request.
        sendMPDCommand("close\n");

        // Wait for connection termination and forcefully close the socket if not done gracefully.
        mTCPSocket->close();
        mTCPSocket->waitForDisconnected(5000);
        if ( mTCPSocket->state() != QAbstractSocket::UnconnectedState) {
            mTCPSocket->abort();
            mTCPSocket->reset();
        }
        mIdling = false;
    }
    if ( mTCPSocket ) {
        mTCPSocket->deleteLater();
        mTCPSocket = 0;
    }
    qDebug() << "Old socket cleared for deletion";
    emit ready();
}



/** return all albums currently availible from connected MPD as MpdAlbum objects,
  * empty list if not connected or no albums are availible */
void NetworkAccess::getAlbums()
{
    emit busy();
    QList<MpdAlbum*> *albums = new QList<MpdAlbum*>();
    if (connected()) {
        /* Start getting list from mpd. If server is new enough try to filter my musicbrainz album id.
         * This helps with albums that have the same name as others (e.g. "Greatest Hits").
         * Not fully implemented yet */

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
        bool emptyAlbum = false;
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
                /* Remove newline at the end */
                response.chop(1);
                /* Parse album name and if server is new enough parse mbid */
                if ( response.startsWith("Album: ") ) {
                    // Append album if name is already set(last album)
                    if ( name != "" || emptyAlbum ) {
                        tempalbum = new MpdAlbum(NULL,name,"",mbid);

                        /* This helps with qml Q_PROPERTY accesses */
                        tempalbum->moveToThread(mQMLThread);
                        /* Set ownership to CppOwnership to guarantee that the GC of qml never deletes this */
                        QQmlEngine::setObjectOwnership(tempalbum, QQmlEngine::CppOwnership);
                        albums->append(tempalbum);
                        emptyAlbum = false;
                    }
                    name = response.right(response.length() - 7);
                    if ( name == "" ) {
                        emptyAlbum = true;
                    }
                }  else if ( response.startsWith("MUSICBRAINZ_ALBUMID:") ) {
                    mbid = response.right(response.length() - 21);
                }
            }
        }
        /* Make sure the last album isn't missed because of loop structure */
        if ( name != "" || emptyAlbum ) {
            tempalbum = new MpdAlbum(NULL,name,"",mbid);
            /* See above */
            tempalbum->moveToThread(mQMLThread);
            /* Set ownership to CppOwnership to guarantee that the GC of qml never deletes this */
            QQmlEngine::setObjectOwnership(tempalbum, QQmlEngine::CppOwnership);
            albums->append(tempalbum);
        }
    }

    //Get album tracks
    qSort(albums->begin(),albums->end(),MpdAlbum::lessThan);

    /* Notify potential busy indicators */
    emit ready();
    /* Emit ready signal and send list with it */
    emit albumsReady((QList<QObject*>*)albums);
}

void NetworkAccess::getArtists()
{
    emit busy();
    /* Requests all artists of the mpd database and send them back with a ready signal */
    emit artistsReady((QList<QObject*>*)getArtists_prv());
    emit ready();

}

/* Private function to fetch and parse artists from mpd */
QList<MpdArtist*> *NetworkAccess::getArtists_prv()
{
    QList<MpdArtist*> *artists = new QList<MpdArtist*>();
    if (connected()) {
        //Start getting list from mpd
        //Send request
        sendMPDCommand("list artist\n");

        //Read & parse all artists until OK send from mpd
        QString response ="";
        MpdArtist *tempartist=NULL;
        QString name;
        MPD_WHILE_PARSE_LOOP
        {
            /* Wait until data is available */
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                /* Read line and chop new line at the end */
                response = QString::fromUtf8(mTCPSocket->readLine());
                response.chop(1);
                /* Parse mpd output */
                if (response.startsWith("Artist: "))
                {
                    name = response.right(response.length()-8);
                    tempartist = new MpdArtist(NULL,name);
                    /* This helps with qml Q_PROPERTY accesses */
                    tempartist->moveToThread(mQMLThread);
                    /* Set ownership to CppOwnership to guarantee that the GC of qml never deletes this */
                    QQmlEngine::setObjectOwnership(tempartist, QQmlEngine::CppOwnership);
                    artists->append(tempartist);
                }

            }
        }
    }
    /* Sort the created list */
    qSort(artists->begin(),artists->end(),MpdArtist::lessThan);
    /* Return the list directly, this will later be send further via signals for multithreading. */
    return artists;
}

/* Tries to authenticate with mpd server. Returns true if successfully authenticated */
bool NetworkAccess::authenticate(QString password)
{
    if (connected()) {

        QTextStream outstream(mTCPSocket);
        outstream.setCodec("UTF-8");
        outstream << "password " << password << "\n";
        outstream.flush();
        mTCPSocket->waitForReadyRead(READYREAD);
        //Check Response
        QString response;
        while (mTCPSocket->canReadLine())
        {
            response += mTCPSocket->readLine();
        }
        QString teststring = response;
        teststring.truncate(2);
        /* Check authentication result here and return */
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
    /* Request all albums for the specific artist and send them away via a signal
     * for multithreading. */
    emit(artistAlbumsReady((QList<QObject*>*)getArtistsAlbums_prv(artist)));
    emit ready();
}

QList<MpdAlbum*> *NetworkAccess::getArtistsAlbums_prv(QString artist)
{
    QList<MpdAlbum*> *albums = new QList<MpdAlbum*>();
    if (connected()) {
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
        bool emptyAlbum = false;

        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
                /* Remove newline at the end */
                response.chop(1);
                if ( response.startsWith("Album: ") ) {
                    // Append album if name is already set(last album)
                    if ( name != "" || emptyAlbum ) {
                        tempalbum = new MpdAlbum(NULL,name,artist,mbid);
                        qDebug() << "Album: " << name;
                        tempalbum->moveToThread(mQMLThread);
                        QQmlEngine::setObjectOwnership(tempalbum, QQmlEngine::CppOwnership);
                        albums->append(tempalbum);
                        emptyAlbum = false;
                    }
                    name = response.right(response.length() - 7);
                    if ( name == "" ) {
                        emptyAlbum = true;
                    }
                }  else if ( response.startsWith("MUSICBRAINZ_ALBUMID:") ) {
                    mbid = response.right(response.length() - 21);
                }
            }
        }
        /* Append last album also */
        if ( name != "" || emptyAlbum) {
            qDebug() << "Album: " << name;
            tempalbum = new MpdAlbum(NULL,name,artist,mbid);
            tempalbum->moveToThread(mQMLThread);
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
    emit trackListReady(getAlbumTracks_prv(album));
    emit ready();
}

QList<MpdTrack*>* NetworkAccess::getAlbumTracks_prv(QString album)
{
    if (connected()) {
        album.replace(QString("\""),QString("\\\""));
        sendMPDCommand(QString("find album \"") + album + "\"\n");
    }
    return parseMPDTracks("");
}

void  NetworkAccess::getAlbumTracks(QString album, QString cartist)
{
    emit busy();
    emit trackListReady(getAlbumTracks_prv(album,cartist));
    emit ready();
}

void  NetworkAccess::getAlbumTracks(QVariant albuminfo)
{
    emit busy();
    QStringList strings = albuminfo.toStringList();
    emit trackListReady(getAlbumTracks_prv(strings[1],strings[0]));
    emit ready();
}

QList<MpdTrack*>*  NetworkAccess::getAlbumTracks_prv(QString album, QString cartist)
{
    if (cartist=="")
    {

        return getAlbumTracks_prv(album);
    }
    if (connected()) {
        album.replace(QString("\""),QString("\\\""));
        sendMPDCommand(QString("find album \"") + album + "\"\n");
    }
    return parseMPDTracks(cartist);
}

void NetworkAccess::getTracks()
{
    emit busy();
    if (connected()) {
        sendMPDCommand("listallinfo\n");
    }
    emit parseMPDTracks("");
    emit ready();
}



void NetworkAccess::getCurrentPlaylistTracks()
{
    emit busy();
    if (connected()) {
        sendMPDCommand("playlistinfo\n");
    }
    emit currentPlaylistReady(parseMPDTracks(""));
    emit ready();
}


void NetworkAccess::getPlaylistTracks(QString name)
{
    emit busy();
    if (connected()) {
        sendMPDCommand(QString("listplaylistinfo \"") + name + "\"\n");
    }
    emit trackListReady(parseMPDTracks(""));
    emit ready();
}


void NetworkAccess::getStatus()
{
    qDebug() << "::getStatus()";
    if (connected()) {
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
            mTCPSocket->waitForReadyRead(READYREAD);
            mLastStatusTimestamp.start();
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
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
                    mLastSyncElapsedTime = elapstr.toUInt();
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
                    if ( playlistversion != mPlaybackStatus->getPlaylistVersion() ) {
                        newSong = true;
                    }
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
                            if ( mPlaybackStatus->getPlaybackStatus() == MPD_STOP) {
                                newSong = true;
                            }
                            mPlaybackStatus->setPlaybackStatus(MPD_PLAYING);
                        }
                        else if (response == "pause") {
                            if ( mPlaybackStatus->getPlaybackStatus() == MPD_STOP) {
                                newSong = true;
                            }
                            mPlaybackStatus->setPlaybackStatus(MPD_PAUSE);
                        }
                        else if (response == "stop") {
                            mPlaybackStatus->setPlaybackStatus(MPD_STOP);
                        }
                    }
                }
                else if (response.startsWith("repeat: ")) {
                    {
                        qDebug() << response;
                        mPlaybackStatus->setRepeat(response.right(response.length()-8) == "1" ? true : false);
                    }
                }
                else if (response.startsWith("random: ")) {
                    {
                        qDebug() << response;
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
            mPlaybackStatus->clearPlayback();
            response = "";
            sendMPDCommand("currentsong\n");
            MPD_WHILE_PARSE_LOOP
            {
                mTCPSocket->waitForReadyRead(READYREAD);
                while (mTCPSocket->canReadLine())
                {
                    response = QString::fromUtf8(mTCPSocket->readLine());
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
    if (connected()) {
        MpdPlaybackState playbackState = getPlaybackState();
        if(playbackState != MPD_STOP) {
            sendMPDCommand("pause\n");
            QString response ="";
            MPD_WHILE_PARSE_LOOP
            {
                mTCPSocket->waitForReadyRead(READYREAD);
                while (mTCPSocket->canReadLine())
                {
                    response = QString::fromUtf8(mTCPSocket->readLine());

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
    if (connected()) {
        sendMPDCommand("stop\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::enableOutput(int nr)
{
    if (connected()) {
        sendMPDCommand(QString("enableoutput ") + QString::number(nr) + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
    }
}

void NetworkAccess::disableOutput(int nr)
{
    if (connected()) {
        sendMPDCommand(QString("disableoutput ") + QString::number(nr) + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
    }
}
void NetworkAccess::updateDB()
{
    if (connected()) {
        sendMPDCommand("update\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::next()
{
    if (connected()) {
        sendMPDCommand("next\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::previous()
{
    if (connected()) {
        sendMPDCommand("previous\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::addAlbumToPlaylist(QString album)
{
    emit busy();
    if (connected()) {
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
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
    }
    emit ready();
    //   getStatus();
}

void NetworkAccess::addArtistAlbumToPlaylist(QString artist, QString album)
{
    emit busy();
    if (connected()) {
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
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

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
    if (connected()) {
        sendMPDCommand(QString("add \"") + fileuri + "\"\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

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
    if (connected()) {
        sendMPDCommand(QString("playlistadd \"") + inputStrings.at(1) + "\" \"" + inputStrings.at(0) + "\"\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

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
    if (connected()) {
        sendMPDCommand(QString("playlistdelete \"") + inputStrings.at(1) + "\" " + inputStrings.at(0) + "\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
    }
    getStatus();
}

void NetworkAccess::playTrackNext(int index)
{
    quint32 currentPosition = getPlaybackID();
    if (connected()) {
        if ( (quint32) index < currentPosition) {
            sendMPDCommand(QString("move ") + QString::number(index) + " " + QString::number(currentPosition) + "\n");
        } else {
            sendMPDCommand(QString("move ") + QString::number(index) + " " + QString::number(currentPosition + 1) + "\n");
        }
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

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
    if (connected()) {
        sendMPDCommand(QString("addid \"") + fileuri + "\" " + QString::number(currentPosition+1) + "\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
    }
    getStatus();
}

//Replace song with uri and plays it back
void NetworkAccess::playFiles(QString fileuri)
{
    clearPlaylist();
    if (connected()) {
        sendMPDCommand(QString("add \"") + fileuri + "\"\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

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
    if (connected()) {
        sendMPDCommand(QString("add \"") + fileuri + "\"\n");
        QString response ="";
        //Clear read buffer
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
        //Get song id in playlist

        
        playTrackByNumber(getPlaylistLength() - 1);
    }
    getStatus();
}

void NetworkAccess::playTrackByNumber(int nr)
{
    if (connected()) {
        sendMPDCommand(QString("play ") + QString::number(nr).toUtf8() + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::deleteTrackByNumer(int nr)
{
    if (connected()) {
        sendMPDCommand(QString("delete " ) + QString::number(nr).toUtf8() + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::seekPosition(int id, int pos)
{
    qDebug() << "seek: " << id << ":" << pos;
    if (connected()) {
        sendMPDCommand(QString("seek ") + QString::number(id).toUtf8() + " " +  QString::number(pos).toUtf8() + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

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
    if (connected()) {
        sendMPDCommand(QString("repeat ") + (repeat ? "1":"0") + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
    }
}

void NetworkAccess::setRandom(bool random)
{
    if (connected()) {
        sendMPDCommand(QString("random ") + (random ? "1":"0") + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
        getStatus();

    }

}

void NetworkAccess::setVolume(int volume)
{
    if (connected()) {
        sendMPDCommand(QString("setvol ") + QString::number(volume).toUtf8() + "\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
    }
}

void NetworkAccess::savePlaylist(QString name)
{
    emit ready();
    if (connected()) {
        sendMPDCommand(QString("save \"") + name + "\"\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

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
    if (connected()) {
        sendMPDCommand(QString("rm \"") + name + "\"\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

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
    if (connected()) {
        sendMPDCommand("listplaylists\n");
        QString response ="";
        QString name;

        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
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
    if (connected()) {
        sendMPDCommand(QString("load \"") + name + "\"\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
    }
    emit ready();
}

void NetworkAccess::playPlaylist(QString name)
{
    emit busy();
    clearPlaylist();
    if (connected()) {
        sendMPDCommand(QString("load \"") + name + "\"\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
    }
    playTrackByNumber(0);
    emit ready();
}

void NetworkAccess::clearPlaylist()
{
    if (connected()) {
        sendMPDCommand("clear\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());

            }
        }
        getStatus();
    }
}

void NetworkAccess::onServerDisconnected()
{
    qDebug() << "Disconnected";
    mPlaylistversion = 0;
    if ( mPlaybackStatus) {
        mPlaybackStatus->clearPlayback();
        mPlaybackStatus->setID(0);
    }
    if (mStatusTimer->isActive())
    {
        mStatusTimer->stop();
    }
    mIdling = false;
    if ( mIdleCountdown->isActive()) {
        mIdleCountdown->stop();
    }

    /* Reset server capabilities */
    pServerInfo.mpd_cmd_list_filter_criteria = false;
    pServerInfo.mpd_cmd_list_group_capabilites = false;
    pServerInfo.mpd_cmd_idle = false;

    emit ready();
}

void NetworkAccess::onServerConnected() {
    qDebug() << "Connected to mpd server";

    mIdling = false;

    if (connected()) {
        //Do host authentication
        mTCPSocket->waitForReadyRead(READYREAD);
        QString response;
        while (mTCPSocket->canReadLine())
        {
            response += mTCPSocket->readLine();
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
                }
            }
        }

        if ( mPassword != "" ) {
            authenticate(mPassword);
        }
        checkServerCapabilities();
        emit ready();
        emit connectionEstablished();
        qDebug() << "Handshake with server done";
    }


    mPlaylistversion = 0;
    if ( mPlaybackStatus) {
        mPlaybackStatus->clearPlayback();
    }
    mIdling = false;

    getStatus();

    mStatusTimer->start(mStatusInterval);
}

quint32 NetworkAccess::getPlayListVersion()
{
    quint32 playlistversion = 0;
    if (connected()) {
        sendMPDCommand("status\n");
        QString response ="";
        QString versionstring;
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
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
    if (connected()) {
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
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
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
                            temptrack->moveToThread(mQMLThread);
                            tempfile->moveToThread(mQMLThread);
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
                        tempfile->moveToThread(mQMLThread);
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
                        tempfile->moveToThread(mQMLThread);
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
                temptrack->moveToThread(mQMLThread);
                tempfile->moveToThread(mQMLThread);
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
    mStatusInterval = ms;
    if (mStatusTimer->isActive())
    {
        mStatusTimer->stop();
        mStatusTimer->start(mStatusInterval);
    }
    if(ms==0){
        mStatusTimer->stop();
    }
}

bool NetworkAccess::connected()
{
    if (mTCPSocket && mTCPSocket->state() != QAbstractSocket::UnconnectedState)
    {
        return true;
    } else {
        return false;
    }
}

void NetworkAccess::onConnectionError()
{
    emit ready();
    mTCPSocket->disconnectFromHost();
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
    if (connected()) {
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
            if (!mTCPSocket->waitForReadyRead(READYREAD))
            {
            }

            while (Q_LIKELY(mTCPSocket->canReadLine()))
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
                // Remove new line
                response.chop(1);
                if (response.startsWith("file: ")) {
                    if (temptrack!=NULL)
                    {
                        // Discard track if artist filter mismatches
                        if (artist==cartist||cartist=="") {
                            temptracks->append(temptrack);
                            artistMBID = "";
                            temptrack->moveToThread(mQMLThread);
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
                temptrack->moveToThread(mQMLThread);
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
    if (connected()) {
        emit busy();
        QString response ="";
        QString tempstring;
        QList<MPDOutput*> *outputlist = new QList<MPDOutput*>();
        QString outputname;
        int outputid = 0;
        bool outputenabled = false;


        sendMPDCommand("outputs\n");
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
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
                    tmpOutput->moveToThread(mQMLThread);
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
    if (connected()) {
        sendMPDCommand(QString("search ") + searchrequest.at(0) + " \"" + searchrequest.at(1) + "\"\n");
    }
    emit trackListReady(parseMPDTracks(""));
    emit ready();
}

void NetworkAccess::setQMLThread(QThread *thread)
{
    if(thread)
    {
        Q_ASSERT ( mQMLThread == 0 );
        mQMLThread = thread;
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
        /* Disabled until database support is finished as well */
        pServerInfo.mpd_cmd_list_filter_criteria = false;//true;
        pServerInfo.mpd_cmd_list_group_capabilites = false;//true;
        qDebug() << "Enable new list features of version 0.19";
    } else {
        pServerInfo.mpd_cmd_list_filter_criteria = false;
        pServerInfo.mpd_cmd_list_group_capabilites = false;
    }

    /*
     * Get allowed commands */
    if (connected()) {
        sendMPDCommand("commands\n");
        QString response ="";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
                response.chop(1);
                response = response.right(response.length()-9);
                qDebug() << response;
                if ( response == "idle") {
                    pServerInfo.mpd_cmd_idle = true;
                }
            }
        }
    }
}

MPDPlaybackStatus *NetworkAccess::getMPDPlaybackStatus() {
    return mPlaybackStatus;
}

MpdPlaybackState NetworkAccess::getPlaybackState()
{
    MpdPlaybackState playbackState = MPD_STOP;
    if (connected()) {
        sendMPDCommand("status\n");
        QString response;
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
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
    quint32 playbackID = 0;
    if (connected()) {
        sendMPDCommand("status\n");
        QString response = "";
        MPD_WHILE_PARSE_LOOP
        {
            qDebug() << response;
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
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
    quint32 playlistLength = 0;
    if (connected()) {
        sendMPDCommand("status\n");
        QString response = "";
        MPD_WHILE_PARSE_LOOP
        {
            mTCPSocket->waitForReadyRead(READYREAD);
            while (mTCPSocket->canReadLine())
            {
                response = QString::fromUtf8(mTCPSocket->readLine());
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
    if ( mIdling ) {
        /* Interpolate status here */
        if ( mPlaybackStatus->getPlaybackStatus() == MPD_PLAYING &&
             mPlaybackStatus->getLength() > mPlaybackStatus->getCurrentTime()) {
            mPlaybackStatus->setCurrentTime(mLastSyncElapsedTime + mLastStatusTimestamp.elapsed()/1000  );
        }
        if ( mLastSyncTime.elapsed() >= RESYNC_TIME) {
            qDebug() << "resyncing mpd status for time drift";
            getStatus();
            goIdle();
        }
    } else {
        qDebug() << "Not idling, polling status";
        getStatus();
        if ( !mIdleCountdown->isActive() && connected() && pServerInfo.mpd_cmd_idle ) {
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
    connect(mTCPSocket,SIGNAL(readyRead()),this,SLOT(onNewNetworkData()));
    if (connected()) {
        QTextStream outstream(mTCPSocket);
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
    disconnect(mTCPSocket,SIGNAL(readyRead()),this,SLOT(onNewNetworkData()));
    mIdling = false;
    qDebug() << "Stop idling";
    /* Start the idling and connect newData signal to slot */
    if (connected()) {
        //Do host authentication
        QTextStream outstream(mTCPSocket);
        outstream.setCodec("UTF-8");
        outstream << "noidle" << "\n";
        outstream.flush();
    }
    QString response;
    mTCPSocket->waitForReadyRead(READYREAD);
    while (mTCPSocket->canReadLine())
    {
        response += mTCPSocket->readLine();
    }
    qDebug() << response;

}

void NetworkAccess::onNewNetworkData()
{
    QString response;
    while (mTCPSocket->canReadLine())
    {
        response += mTCPSocket->readLine();
        if ( mIdling && response.contains("changed") ) {
            disconnect(mTCPSocket,SIGNAL(readyRead()),this,SLOT(onNewNetworkData()));
            qDebug() << "Idle seems to be over";
            mIdling = false;
            getStatus();
        }
        response = "";
    }

}

/*
 * React on changes of sockets connection state. This is required to get notified
 * about connection timeouts for example. This controls the busy indication */
void NetworkAccess::onConnectionStateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "New connection state: " << socketState;
    switch ( socketState ) {
        case QAbstractSocket::UnconnectedState: {
        if ( mTimeoutTimer ) {
            mTimeoutTimer->stop();
            delete(mTimeoutTimer);
            mTimeoutTimer = 0;
        }
        emit ready();
        break;
    }
    case QAbstractSocket::HostLookupState: {
        if ( mTimeoutTimer == 0 ) {
            mTimeoutTimer = new QTimer(this);
            mTimeoutTimer->setInterval(CONNECTION_TIMEOUT);
            connect(mTimeoutTimer,SIGNAL(timeout()),this,SLOT(onConnectionTimeout()));
            mTimeoutTimer->setSingleShot(true);
            mTimeoutTimer->start();
        } else {
            mTimeoutTimer->stop();
            mTimeoutTimer->start();
        }
        emit busy();
        break;
    }
    case QAbstractSocket::ConnectingState : {
        if ( mTimeoutTimer == 0 ) {
            mTimeoutTimer = new QTimer(this);
            mTimeoutTimer->setInterval(CONNECTION_TIMEOUT);
            connect(mTimeoutTimer,SIGNAL(timeout()),this,SLOT(onConnectionTimeout()));
            mTimeoutTimer->setSingleShot(true);
            mTimeoutTimer->start();
        } else {
            mTimeoutTimer->stop();
            mTimeoutTimer->start();
        }
        emit busy();
        break;
    }
    case QAbstractSocket::ConnectedState : {
        if ( mTimeoutTimer ) {
            mTimeoutTimer->stop();
            delete(mTimeoutTimer);
            mTimeoutTimer = 0;
        }
        emit ready();
        break;
    }
    case QAbstractSocket::ClosingState:  {
        if ( mTimeoutTimer ) {
            mTimeoutTimer->stop();
            delete(mTimeoutTimer);
            mTimeoutTimer = 0;
        }
        emit busy();
        break;
    }
    default:
        emit ready();
    }
}

void NetworkAccess::onConnectionTimeout() {
    qDebug() << "Connection attempt timeout";
    mTCPSocket->abort();
}

void NetworkAccess::sendMPDCommand(QString cmd)
{
    if (connected()) {
        /* It is important to cancel the idle command first
         * otherwise MPD disconnects the client */
        if ( mIdling ) {
            cancelIdling();
        }
        QTextStream outstream(mTCPSocket);
        outstream.setCodec("UTF-8");
        outstream << cmd;
        outstream.flush();
    }
}
