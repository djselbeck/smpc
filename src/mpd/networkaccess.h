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
    Q_INVOKABLE bool connectToHost(QString hostname, quint16 port,QString password);
    Q_INVOKABLE bool connected();

    bool authenticate(QString passwort);
    void setUpdateInterval(quint16 ms);
    void seekPosition(int id,int pos);
    void setConnectParameters(QString hostname,int port, QString password);
    void setQmlThread(QThread *thread);

    void registerPlaybackStatus(MPDPlaybackStatus *playbackStatus);


signals:
    void connectionestablished();
    void disconnected();
    void connectionError(QString);
    void currentPlayListReady(QList<QObject*>*);
    void artistsReady(QList<QObject*>*);
    void albumsReady(QList<QObject*>*);
    void filesReady(QList<QObject*>*);
    void artistAlbumsReady(QList<QObject*>*);
    void albumTracksReady(QList<QObject*>*);
    void savedPlaylistsReady(QStringList*);
    void savedplaylistTracksReady(QList<QObject*>*);
    void outputsReady(QList<QObject*>*);
    void searchedTracksReady(QList<QObject*>*);

    void artistsAlbumsMapReady(QMap<MpdArtist*, QList<MpdAlbum*>* > *);

    void startupdateplaylist();
    void finishupdateplaylist();
    void busy();
    void ready();
    void requestExit();

public slots:
    void addTrackToPlaylist(QString fileuri);
    void addTrackToSavedPlaylist(QVariant data);
    void removeTrackFromSavedPlaylist(QVariant data);
    void addTrackAfterCurrent(QString fileuri);
    void addAlbumToPlaylist(QString album);
    void playAlbum(QString album);
    void addArtistAlbumToPlaylist(QString artist,QString album);
    void addArtistAlbumToPlaylist(QVariant albuminfo);
    void playArtistAlbum(QString artist, QString album);
    void playArtistAlbum(QVariant albuminfo);
    void addArtist(QString artist);
    void playArtist(QString artist);
    void playFiles(QString fileuri);
    void playTrack(QString fileuri);
    void playTrackByNumber(int nr);
    void deleteTrackByNumer(int nr);
    void pause();
    void next();
    void previous();
    void stop();
    void updateDB();
    void clearPlaylist();
    void setVolume(int volume);
    void setRandom(bool);
    void setRepeat(bool);
    void disconnectFromServer();
    void connectToHost();
    quint32 getPlayListVersion();
    void getAlbums();
    void getArtists();
    void getTracks();
    void getArtistsAlbums(QString artist);
    void getAlbumTracks(QString album);
    void getAlbumTracks(QString album, QString cartist);
    //Variant [Artist,Album]
    void getAlbumTracks(QVariant albuminfo);

    // Plays current playlist entry (index) next
    void playTrackNext(int index);

    void getArtistAlbumMap();

    void getCurrentPlaylistTracks();
    void getPlaylistTracks(QString name);
    void getDirectory(QString path);
    void getSavedPlaylists();
    void seek(int pos);
    void savePlaylist(QString name);
    void addPlaylist(QString name);
    void playPlaylist(QString name);
    void deletePlaylist(QString name);
    void setUpdateInterval(int ms);
    void exitRequest();
    void enableOutput(int nr);
    void disableOutput(int nr);
    void getOutputs();
    void searchTracks(QVariant request);

protected slots:
    void connectedtoServer();
    void disconnectedfromServer();
    void errorHandle();
    void getStatus();

    void interpolateStatus();

    void goIdle();
    void cancelIdling();
    void newDataInIdle();

private:

    void sendMPDCommand(QString cmd);

    QString mHostname;
    quint16 mPort;
    QString mPassword;
    QTcpSocket* tcpsocket;
    QString mpdversion;
    QTimer *statusupdater;
    QTimer *mIdleCountdown;
    quint16 updateinterval;
    quint32 mPlaylistversion;
    QList<MpdTrack*>* parseMPDTracks(QString cartist);
    QList<MpdArtist*>* getArtists_prv();
    QList<MpdTrack*>* getAlbumTracks_prv(QString album);
    QList<MpdTrack*>* getAlbumTracks_prv(QString album, QString cartist);
    QList<MpdAlbum*>* getArtistsAlbums_prv(QString artist);
    QMap<MpdArtist*, QList<MpdAlbum*>* > *getArtistsAlbumsMap_prv();
    QThread *mQmlThread;

    MpdPlaybackState getPlaybackState();
    quint32 getPlaybackID();
    quint32 getPlaylistLength();

    MPDPlaybackStatus *mPlaybackStatus;

    MpdServerInfo pServerInfo;
    void checkServerCapabilities();

    QTime mLastSyncTime;

    bool mIdling;
};

#endif // NETWORKACCESS_H
