#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QStack>
#include "mpdtrack.h"

#include <QtDeclarative>
#include <QDeclarativeContext>
#include <QDeclarativeListReference>
#include "networkaccess.h"
#include "commondebug.h"
#include "qthreadex.h"
#include "serverprofile.h"
#include "artistmodel.h"
#include "albummodel.h"
#include "mpdoutput.h"



class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    Controller(QDeclarativeView *viewer,QObject *parent = 0);
    void connectSignals();
public slots:
    void focusChanged(QWidget *old, QWidget *now);

signals:
    void sendPopup(QVariant text);
    void sendStatus(QVariant status);
    void playlistUpdated();
    void filesModelReady();
    void getFiles(QString path);
    void requestConnect();
    void requestDisconnect();
    void albumsReady();
    void artistsReady();
    void albumTracksReady();
    void artistAlbumsReady();
    void savedPlaylistsReady();
    void savedPlaylistReady();
    void outputsReady();
    void serverProfilesUpdated();
    void setVolume(int);
    void setUpdateInterval(int);
    void showWelcome();
    void requestExit();
    void searchedTracksReady();
    void addURIToPlaylist(QString);



private:
    QDeclarativeView *viewer;
    NetworkAccess *netaccess;
    QString hostname,password,profilename;
    quint16 port;
    quint32 playlistversion;
    int currentsongid;
    int volume;
    int lastplaybackstate;
    QThreadEx *networkthread;
    QList<ServerProfile*> *serverprofiles;
    void readSettings();
    void writeSettings();
    QTimer volDecTimer,volIncTimer;
    QList<MpdAlbum*> *albumlist;
    QList<MpdArtist*> *artistlist;
    ArtistModel *artistmodelold;
    AlbumModel *albumsmodelold;
    QList<MpdTrack*> *trackmodel;
    QList<MpdTrack*> *playlist;
    QList<MpdTrack*> *searchedtracks;
    QList<MPDOutput*> *outputs;
    QStack<QList<QObject*>*> *filemodels;
    QThread *oldnetthread;

private slots:
    void requestCurrentPlaylist();
    void requestAlbums();
    void requestArtists();
    void requestArtistAlbums(QString artist);
    void requestAlbum(QVariant array);
    void requestFilePage(QString);
    void requestFileModel(QString);
    void seek(int);
    void incVolume();
    void decVolume();
    void updateStatus(status_struct status);
    void mediaKeyHandle(int key);
    void mediaKeyPressed(int key);
    void mediaKeyReleased(int key);
    /*Privates*/
    void connectedToServer();
    void disconnectedToServer();
    void updateAlbumsModel(QList<QObject*>* list);
    void updateArtistsModel(QList<QObject*>* list);
//    void updateArtistAlbumsModel(QList<QObject*>* list);
    void updatePlaylistModel(QList<QObject*>* list);
    void updateFilesModel(QList<QObject*>* list);
    void updateAlbumTracksModel(QList<QObject*>* list);
    void updateOutputsModel(QList<QObject*>* list);
    void updateSearchedTracks(QList<QObject*>* list);
    void setHostname(QString hostname);
    void setPassword(QString password);
    void setPort(int port);
    void connectToServer();
    void quit();
    void newProfile(QVariant profile);
    void changeProfile(QVariant profile);
    void deleteProfile(int index);
    void connectProfile(int index);
    void updateSavedPlaylistsModel(QStringList*);
    void updateSavedPlaylistModel(QList<QObject*>* list);
    void applicationActivate();
    void applicationDeactivate();
    void fileStackPop();
    void cleanFileStack();
    void exitRequest();
    void addlastsearchtoplaylist();






};

#endif // CONTROLLER_H