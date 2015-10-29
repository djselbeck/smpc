#ifndef NETWORKACCESS_H
#define NETWORKACCESS_H

#include <QObject>
#include <QtNetwork>
#include <QQmlEngine>
#include <mpd/mpdalbum.h>
#include <mpd/mpdartist.h>
#include <mpd/mpdtrack.h>
#include <mpd/mpdfileentry.h>
#include <mpd/mpdoutput.h>
#include <mpd/mpdplaybackstatus.h>
#include <mpd/mpdcommon.h>

#include <common.h>

// Timeout value for network communication
#define READYREAD 15000
#define IDLEWAIT 2000
#define RESYNC_TIME 30 * 1000

class MpdAlbum;
class MpdArtist;
class MpdTrack;
class MpdFileEntry;

class NetworkAccess : public QThread
{
    Q_OBJECT
    
public:
    explicit NetworkAccess(QObject *parent = 0);
    /**
     * @brief connectToHost Initiates a new connection to the given mpd server.
     * @param hostname to connect to
     * @param port used to connect to
     * @param password used to authenticate with. Can be empty if not used on server.
     */
    Q_INVOKABLE void connectToHost(QString hostname, quint16 port,QString password = "");

    /**
     * @brief Checks if connection to server exists
     * @return true if connected, false otherwise
     *
    */
    Q_INVOKABLE bool connected();

    /**
     * @brief setUpdateInterval Used to relax update rate to preserve energy.
     * For example no need to update status every second if window is not visible.
     * @param ms Update interval in ms.
     */
    void setUpdateInterval(quint16 ms);

    /**
     * @brief setConnectParameters Sets parameters for later connect call.
     * @param hostname to connect to
     * @param port used to connect
     * @param password used to authenticate on server. Can be left empty.
     */
    void setConnectParameters(QString hostname,int port, QString password = "");

    /**
     * @brief setQMLThread Sets the Thread of the QMLEngine used to access certain Q_PROPERTYs(MPDOutputs)
     * @param thread The thread used by the qml part.
     */
    void setQMLThread(QThread *thread);

    /**
     * @brief registerPlaybackStatus This registers the playbackStatus object which is used
     * by a controller to inform the user of the current playback status.
     * This function must only be called once.
     * @param playbackStatus Playbackstatus object to register.
     */
    void registerPlaybackStatus(MPDPlaybackStatus *playbackStatus);


signals:
    /* Connection signals */
    /**
     * @brief connectionEstablished signals that the connection to the mpd server
     * was established successfully.
     */
    void connectionEstablished();
    /**
     * @brief disconnected Signals that the connection to the mpd server was lost.
     */
    void disconnected();
    /**
     * @brief connectionError An error occured on the network connection.
     */
    void connectionError(QString);

    /* Database signals */
    /**
     * @brief artistsReady Signals that the list with MPDArtists is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void artistsReady(QList<QObject*>*);
    /**
     * @brief albumsReady Signals that the list with MPDAlbums is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void albumsReady(QList<QObject*>*);
    /**
     * @brief filesReady Signals that the list with MPDFileentrys is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void filesReady(QList<QObject*>*);
    /**
     * @brief artistAlbumsReady Signals that the list with MPDAlbums for a specific artist is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void artistAlbumsReady(QList<QObject*>*);

    /* Track based lists */
    /**
     * @brief albumTracksReady Signals that the list with MPDTracks from an album is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void albumTracksReady(QList<QObject*>*);
    /**
     * @brief searchedTracksReady Signals that the list of MPDTracks for the last search is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void searchedTracksReady(QList<QObject*>*);
    /**
     * @brief currentPlaylistReady Signals that the current active playlist is received. This signal
     * gets emitted everytime the server changes its playlist.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void currentPlaylistReady(QList<QObject*>*);

    /**
     * @brief savedPlaylistsReady Signals that the List of playlist names is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void savedPlaylistsReady(QStringList*);
    /**
     * @brief savedPlaylistTracksReady Signals that the list of MPDTracks of the requested saved playlist is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void savedPlaylistTracksReady(QList<QObject*>*);

    /**
     * @brief artistsAlbumsMapReady Signals that the map of artists<-albums is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void artistsAlbumsMapReady(QMap<MpdArtist*, QList<MpdAlbum*>* > *);

    /**
     * @brief outputsReady Signals that the list of MPDOutputs is ready.
     * Receiver needs to take care of clean up after use or receiving a new one.
     */
    void outputsReady(QList<QObject*>*);

    /**
     * @brief busy Emitted when a large/long network request is running. Can be used for busy indicators.
     */
    void busy();
    /**
     * @brief ready Emitted when a network request is finished. Can be used for busy indicators.
     */
    void ready();
    /**
     * @brief requestExit Emitted when the network handler is ready to quit. This is when the connection is disconnected after the request
     * to exit.
     */
    void requestExit();

public slots:
    /**
     * @brief disconnectFromServer Request to disconnect from current server if connected.
     */
    void disconnectFromServer();
    /**
     * @brief connectToHost Request to server previously set with setConnectParameters();.
     */
    void connectToHost();

    /*
     * Simple control slots
    */
    /**
     * @brief pause Toggles play/pause. If stopped tries to resume playback at last played track.
     */
    void pause();
    /**
     * @brief next Jumps to next song in playlist if any.
     */
    void next();
    /**
     * @brief previous Jumps to previous song in playlist if any.
     */
    void previous();
    /**
     * @brief stop Stops playback
     */
    void stop();
    /**
     * @brief seekPosition Seeks to position in track with id and at pos in seconds.
     * @param id Song to seek in. Playlist position.
     * @param pos Position in seconds to seek to in the song.
     */
    void seekPosition(int id,int pos);
    /**
     * @brief seek Seeks inside currently playing song.
     * @param pos Position in seconds to seek to.
     */
    void seek(int pos);

    /**
     * @brief setVolume Sets the volume of the server.
     * @param volume 0-100 to set.
     */
    void setVolume(int volume);

    /**
     * @brief setRandom Enables/disables random function of connected server.
     * @param random Set means enable
     */
    void setRandom(bool random);
    /**
     * @brief setRepeat Enables/disables repeat function of connected server.
     * @param repeat Set means enable
     */
    void setRepeat(bool repeat);

    /*
     * Database request/query slots
    */
    /**
     * @brief getAlbums Requests to fetch all albums of the connected mpd server.
     * If successful the signal albumsReady() is emitted.
     */
    void getAlbums();
    /**
     * @brief getArtists Requests to fetch all artists of the connected mpd server.
     * If successful the signal artistsReady() is emitted.
     */
    void getArtists();
    /**
     * @brief getTracks Requests all tracks of the connected mpd server.
     * Handle with care because this could be a long list.
     * If successful the signal FIXME is emitted.
     */
    void getTracks();
    /**
     * @brief getArtistsAlbums Requests all albums of artist.
     * If successful the signal artistAlbumsReady() is emitted.
     * @param artist to filter with.
     */
    void getArtistsAlbums(QString artist);
    /**
     * @brief getAlbumTracks Requests all album tracks of album.
     * If successful the signal albumTracksReady() is emitted.
     * @param album to filter with.
     */
    void getAlbumTracks(QString album);
    /**
     * @brief getAlbumTracks Requests all the artist albums track.
     * If successful the signal albumTracksReady() is emitted.
     * @param album to filter with.
     * @param cartist artist to filter with.
     */
    void getAlbumTracks(QString album, QString cartist);

    /**
     * @brief getAlbumTracks Requests all artist albums tracks. Convenient for qml calls
     * because of the variant format [Artist,Album].
     * If successful the signal albumTracksReady() is emitted.
     * @param albuminfo [Artist,Album] to filter with.
     */
    void getAlbumTracks(QVariant albuminfo);
    /**
     * @brief getArtistAlbumMap Requests all artists with corresponding albums.
     * Be aware that this will take some time usually and provides some load on the server.
     * If successful the signal artistsAlbumsMapReady() is emitted.
     */
    void getArtistAlbumMap();

    /**
     * @brief getCurrentPlaylistTracks Request all tracks of the current active playlist.
     * If successful the signal currentPlaylistReady() is emitted.
     */
    void getCurrentPlaylistTracks();
    /**
     * @brief getPlaylistTracks Requests all the tracks for a specific saved playlist.
     * If successful the signal savedPlaylistTracksReady() is emitted.
     * @param name Name of the playlist which tracks should be fetched.
     */
    void getPlaylistTracks(QString name);
    /**
     * @brief getDirectory Requests MPDFileEntrys for a specific path.
     * If successful the signal filesReady() is emitted.
     * @param path
     */
    void getDirectory(QString path);
    /**
     * @brief getSavedPlaylists Requests the list of saved playlists.
     * If successful the signal savedPlaylistsReady() is emitted.
     */
    void getSavedPlaylists();

    /**
     * @brief searchTracks Searches for specific tracks with a search criteria.
     * If successful the signal searchedTracksReady() is emitted.
     * @param request [criteria,value]. For criterias look in the mpd protocol definition.
     */
    void searchTracks(QVariant request);


    /*
     * Add/play requests of database items
     */
    /**
     * @brief addAlbumToPlaylist Requests to add album to the playlist.
     * @param album to add to the current playlist.
     */
    void addAlbumToPlaylist(QString album);
    /**
     * @brief addArtist Add all albums of an artists to the playlist.
     * @param artist which is added to the current playlist.
     */
    void addArtist(QString artist);
    /**
     * @brief addTrackToPlaylist Add a specific file to the current playlist.
     * @param fileuri URI of the file to be added. Can be remote like http,....
     */
    void addTrackToPlaylist(QString fileuri);
    /**
     * @brief addTrackAfterCurrent Adds a specific file to the current playlist AFTER the currently playing one.
     * @param fileuri URI of the file to be added. Can be remote like http,....
     */
    void addTrackAfterCurrent(QString fileuri);
    /**
     * @brief addTrackToSavedPlaylist Adds an track to a existing saved playlist. Useful for playlist management.
     * @param data
     */
    void addTrackToSavedPlaylist(QVariant data);
    /**
     * @brief addArtistAlbumToPlaylist Adds an artist album to the current playlist.
     * @param artist to filter with.
     * @param album to filter with.
     */
    void addArtistAlbumToPlaylist(QString artist,QString album);
    /**
     * @brief addArtistAlbumToPlaylist Adds an artist album to the current playlist.
     * @param albuminfo [Artist,Album] to filter with.
     */
    void addArtistAlbumToPlaylist(QVariant albuminfo);

    /**
     * @brief playAlbum Clears the current playlist and adds the specified album and starts playing it.
     * @param album Album to playAlbum();
     */
    void playAlbum(QString album);
    /**
     * @brief playArtist Clears the current playlist and adds the specified albums of the artists and starts playing them.
     * @param artist to play albums from.
     */
    void playArtist(QString artist);
    /**
     * @brief playFiles Replace playlist with the specific files and plays them.
     * @param fileuri File to play.
     */
    void playFiles(QString fileuri);
    /**
     * @brief playTrack Appends the specific file to the playlist and plays it.
     * @param fileuri File to play.
     */
    void playTrack(QString fileuri);
    /**
     * @brief playTrackByNumber Jumps to specified playlist position.
     * @param nr Playlist position to play.
     */
    void playTrackByNumber(int nr);
    /**
     * @brief playArtistAlbum Replaces & plays the current playlist with an specific artistalbum.
     * @param artist to filter with.
     * @param album to filter with
     */
    void playArtistAlbum(QString artist, QString album);
    /**
     * @brief playArtistAlbum Replaces & plays the current playlist with an specific artistalbum.
     * @param albuminfo [Artist,Album] to filter with.
     */
    void playArtistAlbum(QVariant albuminfo);

    /**
     * @brief deleteTrackByNumer Deletes one track from the current playlist.
     * @param nr index of track to remove.
     */
    void deleteTrackByNumer(int nr);

    /**
     * @brief removeTrackFromSavedPlaylist Removes a track from a saved playlist. Useful for playlist management.
     * @param data [index,playlistName]
     */
    void removeTrackFromSavedPlaylist(QVariant data);

    /**
     * @brief updateDB Triggers the update of the database on the connected mpd server.
     */
    void updateDB();
    /**
     * @brief clearPlaylist Clears the current playlist.
     */
    void clearPlaylist();
    /**
     * @brief getPlayListVersion Fetches the playlist version of the current playlist.
     * @return Version of the playlist.
     */
    quint32 getPlayListVersion();

    /**
     * @brief playTrackNext Moves a specific track to the index after the currently playing one.
     * @param index of track to move.
     */
    void playTrackNext(int index);

    /**
     * @brief savePlaylist Saves the currently active playlist.
     * @param name of the playlist to save.
     */
    void savePlaylist(QString name);
    /**
     * @brief addPlaylist Adds a saved playlist to the current playlist.
     * @param name of the playlist to add.
     */
    void addPlaylist(QString name);
    /**
     * @brief playPlaylist Clears current playlist, adds saved playlist and start playing it.
     * @param name of the playlist
     */
    void playPlaylist(QString name);
    /**
     * @brief deletePlaylist Deletes a saved playlist with a name.
     * @param name of the playlist.
     */
    void deletePlaylist(QString name);
    /**
     * @brief setUpdateInterval Sets the status update interval. Useful to reduce energy consumption.
     * For example a hidden window doesn't have to update the mpd status every second.
     * @param ms
     */
    void setUpdateInterval(int ms);

    /**
     * @brief exitRequest Signals the server that the application requests to quit.
     * Triggers an disconnect from the server.
     */
    void exitRequest();

    /**
     * @brief enableOutput Enables a specific output of the mpd server.
     * @param nr index of the output to enable.
     */
    void enableOutput(int nr);
    /**
     * @brief disableOutput Disables a specific output of the mpd server.
     * @param nr index of the output to disable.
     */
    void disableOutput(int nr);
    /**
     * @brief getOutputs Requests a list of possible outputs from the mpd server.
     * If successful the signal outputsReady() is emitted.
     */
    void getOutputs();

protected slots:
    /**
     * @brief onServerConnected Handles the further connection procedure after the
     * tcpsocket is connected.
     */
    void onServerConnected();
    /**
     * @brief onServerDisconnected Handles the disconnect procedure.
     */
    void onServerDisconnected();
    /**
     * @brief onConnectionError Handles connection errors of the socket.
     */
    void onConnectionError();

    /**
     * @brief getStatus Queries the current status of the mpd server.
     */
    void getStatus();

    /**
     * @brief interpolateStatus Checks if network is idling. If it is than the current song
     * status is interpolated. If it is not idling it starts the idle countdown and queries the
     * current status from the connected mpd server.
     */
    void interpolateStatus();

    /**
     * @brief goIdle Triggers the idling of the network connection.
     */
    void goIdle();

    /**
     * @brief cancelIdling Cancels the idling phase of the mpd connection. This HAS
     * to be called if the current state is idling otherwise the mpd server kicks the
     * connection.
     */
    void cancelIdling();
    /**
     * @brief onNewNetworkData Handles new network data during idle procedure. This means
     * the status of the mpd server has changed and an active status quering is neccessary.
     */
    void onNewNetworkData();

private:
    /**
     * @brief mHostname hostname of the server to connect/connected to
     */
    QString mHostname;
    /**
     * @brief mPort Port of the mpd server
     */
    quint16 mPort;
    /**
     * @brief mPassword If set password which is used to authenticate
     * to mpd server.
     */
    QString mPassword;

    /**
     * @brief mTCPSocket Socket object which handles all the network connection stuff.
     * Moved to separate thread for performance reasons.
     */
    QTcpSocket* mTCPSocket;

    /**
     * @brief mStatusTimer Timer to query the current status. If idle is supported & active
     * this interpolates the position in the currently playing track (if any)
     */
    QTimer *mStatusTimer;

    /**
     * @brief mIdleCountdown Wait timer to countdown active status quering for some time
     * after server notifies about some changes in status,playlist,etc.
     */
    QTimer *mIdleCountdown;
    /**
     * @brief mStatusInterval Interval to update the current playback status.(Interpolate if idling)
     */
    quint16 mStatusInterval;

    /**
     * @brief mPlaylistversion Version id of playlist of the connected mpd server. This is used
     * to determine if the playlist has changed. If it has it gets refetched.
     */
    quint32 mPlaylistversion;

    /**
     * @brief mQmlThread Thread of the qml engine which is used for some objects (MPDOutputs) so that QML
     * can access Q_PROPERTYs (crashes otherwise)
     */
    QThread *mQMLThread;

    /**
     * @brief mPlaybackStatus Current playback status object. Needs to be set!
     * If not set many functions will fail.
     */
    MPDPlaybackStatus *mPlaybackStatus;

    /**
     * @brief pServerInfo Holds information for the currently connected server. Like capabilities, version
     */
    MpdServerInfo pServerInfo;

    /**
     * @brief mLastSyncTime Time of last real status update. This is used to minimze time drift between
     * interpolated song position and real position every RESYNC_TIME ms.
     */
    QTime mLastSyncTime;

    bool mIdling;

    /* Private methods */
    /**
     * @brief parseMPDTracks Functions parses and returns MPDs returned tracks
     * @param cartist Filter criteria. Only tracks with cartist as artist are returned.
     * @return Tracklist of parsed MPD tracks
     */
    QList<MpdTrack*>* parseMPDTracks(QString cartist);

    /**
     * @brief getArtists_prv Fetches a list of MPDArtists
     * @return List of fetched MPDartists
     */
    QList<MpdArtist*>* getArtists_prv();

    /**
     * @brief getAlbumTracks_prv Fetches all the MPDtracks for the specific album
     * @param album Album to get tracks from
     * @return Tracklist as objects
     */
    QList<MpdTrack*>* getAlbumTracks_prv(QString album);

    /**
     * @brief getAlbumTracks_prv Fetches all the MPDtracks for the specific artists album
     * @param album Albumname of album to get tracks from
     * @param cartist Artistname of album to get tracks from
     * @return Tracklist as objects
     */
    QList<MpdTrack*>* getAlbumTracks_prv(QString album, QString cartist);

    /**
     * @brief getArtistsAlbums_prv Fetches all the albums belonging to artist
     * @param artist Artist to get albums from
     * @return Album list of artist
     */
    QList<MpdAlbum*>* getArtistsAlbums_prv(QString artist);

    /**
     * @brief getArtistsAlbumsMap_prv Fetches a map of all artists and all belonging
     * albums to this artist. Useful for cover fetching.
     * @return Map with all artists/albums
     */
    QMap<MpdArtist*, QList<MpdAlbum*>* > *getArtistsAlbumsMap_prv();

    /**
     * @brief checkServerCapabilities
     * Determines which features are available for the connected server
     */
    void checkServerCapabilities();

    /**
     * @brief getPlaybackState Queries the current playbackstate (pause,playing,stopped) of
     * mpd server.
     * @return Current playback state of the connected mpd server
     */
    MpdPlaybackState getPlaybackState();

    /**
     * @brief getPlaybackID Queries the current playlist position.
     * @return The number of the currently playing track. Position in playlist
     */
    quint32 getPlaybackID();

    /**
     * @brief getPlaylistLength Queries the number of elements in playlist.
     * @return The number of elements in current playlist.
     */
    quint32 getPlaylistLength();

    /**
     * @brief sendMPDCommand Sends an command to connected mpd server.
     * If connection is currently idling this is cancelled before.
     * @param cmd Command or command string to send to connected mpd server.
     */
    void sendMPDCommand(QString cmd);

    /**
     * @brief authenticate Used to authenticate to the mpd server
     * @param passwort used to authenticate
     * @return
     */
    bool authenticate(QString passwort);
};

#endif // NETWORKACCESS_H
