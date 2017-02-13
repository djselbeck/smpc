#include "controller.h"
Controller::Controller(QObject *parent) : QObject(parent)
{

}

Controller::Controller(QQuickView *viewer,QObject *parent) : QObject(parent),mQuickView(viewer),mHostname(""),mPassword(""),mPort(6600)
{
    mImgDB = new ImageDatabase();
    mQMLImgProvider = new QMLImageProvider(mImgDB);

    mNetAccess = new NetworkAccess(0);
    mNetAccess->setUpdateInterval(1000);
    mPlaybackStatus = mNetAccess->getMPDPlaybackStatus();

    mNetworkThread = new QThread(this);
    mDBThread = new QThread(this);
    mImgDB->moveToThread(mDBThread);
    mNetAccess->moveToThread(mNetworkThread);
    mNetworkThread->start();
    mDBThread->start();

    mPlaylist = new PlaylistModel(mImgDB,this);
    mOtherTracks = new PlaylistModel(mImgDB,this);

//    mStreamPlayer = new StreamPlayer(this);

    mCurrentSongID=0;
    mPlaylistVersion = 0;
    mOutputs = 0;
    mOldArtistModel = 0;
    mOldAlbumModel = 0;
    mSavedPlaylists = 0;
    mServerProfiles = 0;
    mLastPlaybackState = MPD_STOP;
    connectSignals();
    readSettings();
    qmlRegisterType<MpdArtist>();
    qmlRegisterType<MpdAlbum>();
    qmlRegisterType<ServerProfile>();
    qRegisterMetaType<MpdAlbum>("MpdAlbum");
    qRegisterMetaType<MpdArtist>("MpdArtist");
    volIncTimer.setInterval(250);
    volDecTimer.setInterval(250);
    mWasConnected = false;
    mFileModels = new QStack<FileModel*>();
    // Set empty qml properties for later usage
    mQuickView->rootContext()->setContextProperty("versionstring",QVariant::fromValue(QString(VERSION)));
    mQuickView->rootContext()->setContextProperty("coverstring","");
    mQuickView->rootContext()->setContextProperty("artistInfoText","");
    mQuickView->rootContext()->setContextProperty("albumInfoText","");
    mQuickView->rootContext()->setContextProperty("artistsModel",0);
    mQuickView->rootContext()->setContextProperty("albumsModel",0);
    mQuickView->rootContext()->setContextProperty("savedPlaylistsModel",0);
    mQuickView->rootContext()->setContextProperty("outputsModel",0);
    mQuickView->rootContext()->setContextProperty("playlistModel",mPlaylist);
    mQuickView->rootContext()->setContextProperty("tracksModel",mOtherTracks);


    mQuickView->rootContext()->setContextProperty("mpd_status",mPlaybackStatus);

    viewer->engine()->addImageProvider("imagedbprovider",mQMLImgProvider);
    mNetAccess->setQMLThread(viewer->thread());
    //Start auto connect
    for(int i = 0;i<mServerProfiles->rowCount();i++)
    {
        if(mServerProfiles->get(i)->getAutoconnect())
        {
            connectProfile(i);
            break;
        }
    }
    mDBStatistic = 0;
    mApplicationActive = true;

    emit requestDBStatistic();
}

Controller::~Controller()
{
    qDebug() << "clearing controller";
    // Clear up the mess
    mNetworkThread->quit();
    mNetworkThread->wait();
    delete(mNetworkThread);
    delete(mNetAccess);

    // Close local database
    mDBThread->quit();
    mDBThread->wait();
    delete(mDBThread);
    delete(mImgDB);
    delete(mQMLImgProvider);

    if (mServerProfiles)
        delete(mServerProfiles);

    if (mOldAlbumModel)
        delete(mOldAlbumModel);
    if(mOldArtistModel)
        delete(mOldArtistModel);
    if(mPlaylist)
        delete(mPlaylist);
    if(mOtherTracks)
        delete(mOtherTracks);

    if(mOutputs) {
        qDeleteAll(*mOutputs);
        delete(mOutputs);
    }
    if(mFileModels) {
        qDeleteAll(*mFileModels);
        delete(mFileModels);
    }
    if(mDBStatistic)
        delete(mDBStatistic);

    qDebug() << "everything cleared up nicely";
}


void Controller::updateFilesModel(QList<QObject*>* list)
{
    if(list->length()>0)
    {
        FileModel *model = new FileModel((QList<MpdFileEntry*>*)list,mImgDB,this);
        QQmlEngine::setObjectOwnership(model,QQmlEngine::CppOwnership);
        mQuickView->rootContext()->setContextProperty("filesModel",model);
        mFileModels->push(model);
        emit filesModelReady();
    }

}

void Controller::updateSavedPlaylistsModel(QStringList *list)
{
    mQuickView->rootContext()->setContextProperty("savedPlaylistsModel",0);
    if ( mSavedPlaylists ) {
        delete(mSavedPlaylists);
        mSavedPlaylists = 0;
    }
    mQuickView->rootContext()->setContextProperty("savedPlaylistsModel",QVariant::fromValue(*list));
    mSavedPlaylists = list;
    emit savedPlaylistsReady();

}


void Controller::updateArtistsModel(QList<QObject*>* list)
{
    mQuickView->rootContext()->setContextProperty("artistsModel",0);
    if(mOldArtistModel!=0)
    {
        delete(mOldArtistModel);
        mOldArtistModel = 0;
    }
    //ArtistModel *model = new ArtistModel((QList<MpdTrack*>*)list,this);
    //    artistlist = (QList<MpdArtist*>*)list;
    ArtistModel *model = new ArtistModel((QList<MpdArtist*>*)list,mImgDB,this);
    QQmlEngine::setObjectOwnership(model,QQmlEngine::CppOwnership);
    mOldArtistModel = model;
    mQuickView->rootContext()->setContextProperty("artistsModel",model);
    emit artistsReady();
}

//void Controller::updateArtistAlbumsModel(QList<QObject*>* list)
//{
//    viewer->rootContext()->setContextProperty("albumsModel",QVariant::fromValue(*list));
//    emit artistAlbumsReady();
//}

void Controller::updateAlbumsModel(QList<QObject*>* list)
{
    mQuickView->rootContext()->setContextProperty("albumsModel",0);
    if(mOldAlbumModel!=0)
    {
        delete(mOldAlbumModel);
        mOldAlbumModel = 0;
    }
    AlbumModel *model = new AlbumModel((QList<MpdAlbum*>*)list,mImgDB,getLastFMArtSize(mDownloadSize),mDownloadEnabled, this);
    QQmlEngine::setObjectOwnership(model,QQmlEngine::CppOwnership);
    mOldAlbumModel = model;

    mQuickView->rootContext()->setContextProperty("albumsModel",model);
    emit albumsReady();
}

void Controller::updateOutputsModel(QList<QObject*>* list)
{
    mQuickView->rootContext()->setContextProperty("outputsModel",0);
    if(mOutputs!=0)
    {
        delete(mOutputs);
    }
    mOutputs = (QList<MPDOutput*>*)list;

    mQuickView->rootContext()->setContextProperty("outputsModel",QVariant::fromValue(*list));
    emit outputsReady();
}


void Controller::connectSignals()
{
    QObject *item = (QObject *)mQuickView->rootObject();
    qRegisterMetaType<MPDPlaybackStatus*>("MPDPlaybackStatus*");
    qRegisterMetaType<QList<MpdTrack*>*>("QList<MpdTrack*>*");
    qRegisterMetaType<QList<MpdAlbum*>*>("QList<MpdAlbum*>*");
    qRegisterMetaType<QList<MpdArtist*>*>("QList<MpdArtist*>*");
    qRegisterMetaType<QList<MpdFileEntry*>*>("QList<MpdFileEntry*>*");
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
    qRegisterMetaType<MpdPlaybackState>("MpdPlaybackState");


    connect(item,SIGNAL(setHostname(QString)),this,SLOT(setHostname(QString)));
    connect(item,SIGNAL(setPassword(QString)),this,SLOT(setPassword(QString)));
    connect(item,SIGNAL(setPort(int)),this,SLOT(setPort(int)));
    connect(item,SIGNAL(connectToServer()),this,SLOT(connectToServer()));
    connect(item,SIGNAL(requestCurrentPlaylist()),mNetAccess,SLOT(getCurrentPlaylistTracks()));
    connect(item,SIGNAL(requestArtists()),mNetAccess,SLOT(getArtists()));

    connect(item,SIGNAL(requestArtistAlbums(QString)),mNetAccess,SLOT(getArtistsAlbums(QString)));
    connect(item,SIGNAL(requestAlbums()),mNetAccess,SLOT(getAlbums()));
    connect(item,SIGNAL(requestFilesPage(QString)),this,SLOT(requestFilePage(QString)));
    //  connect(item,SIGNAL(requestCurrentPlaylist()),this,SLOT(requestCurrentPlaylist()));
    connect(item,SIGNAL(playPlaylistTrack(int)),mNetAccess,SLOT(playTrackByNumber(int)));
    connect(item,SIGNAL(deletePlaylistTrack(int)),mNetAccess,SLOT(deleteTrackByNumer(int)));
    connect(item,SIGNAL(requestAlbum(QVariant)),mNetAccess,SLOT(getAlbumTracks(QVariant)));
    connect(item,SIGNAL(stop()),mNetAccess,SLOT(stop()));
    connect(item,SIGNAL(play()),mNetAccess,SLOT(pause()));
    connect(item,SIGNAL(next()),mNetAccess,SLOT(next()));
    connect(item,SIGNAL(prev()),mNetAccess,SLOT(previous()));
    connect(item,SIGNAL(deletePlaylist()),mNetAccess,SLOT(clearPlaylist()));
    connect(item,SIGNAL(addAlbum(QVariant)),mNetAccess,SLOT(addArtistAlbumToPlaylist(QVariant)));
    connect(item,SIGNAL(playAlbum(QVariant)),mNetAccess,SLOT(playArtistAlbum(QVariant)));
    connect(item,SIGNAL(addFiles(QString)),mNetAccess,SLOT(addTrackToPlaylist(QString)));
    connect(item,SIGNAL(seek(int)),mNetAccess,SLOT(seek(int)));
    connect(item,SIGNAL(setVolume(int)),mNetAccess,SLOT(setVolume(int)));
    connect(item,SIGNAL(addArtist(QString)),mNetAccess,SLOT(addArtist(QString)));
    connect(item,SIGNAL(playArtist(QString)),mNetAccess,SLOT(playArtist(QString)));
    connect(item,SIGNAL(savePlaylist(QString)),mNetAccess,SLOT(savePlaylist(QString)));
    connect(item,SIGNAL(deleteSavedPlaylist(QString)),mNetAccess,SLOT(deletePlaylist(QString)));
    connect(item,SIGNAL(requestSavedPlaylists()),mNetAccess,SLOT(getSavedPlaylists()));
    connect(mNetAccess,SIGNAL(savedPlaylistsReady(QStringList*)),this,SLOT(updateSavedPlaylistsModel(QStringList*)));
    connect(mNetAccess,SIGNAL(albumsReady(QList<QObject*>*)),this,SLOT(updateAlbumsModel(QList<QObject*>*)));
    connect(mNetAccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(updateArtistsModel(QList<QObject*>*)));
    connect(mNetAccess,SIGNAL(artistAlbumsReady(QList<QObject*>*)),this,SLOT(updateAlbumsModel(QList<QObject*>*)));
    connect(mNetAccess,SIGNAL(filesReady(QList<QObject*>*)),this,SLOT(updateFilesModel(QList<QObject*>*)));
    connect(mNetAccess,SIGNAL(connectionEstablished()),this,SLOT(connectedToServer()));
    connect(mNetAccess,SIGNAL(disconnected()),this,SLOT(disconnectedToServer()));
    connect(mNetAccess,SIGNAL(busy()),item,SLOT(busy()));
    connect(mNetAccess,SIGNAL(ready()),item,SLOT(ready()));
    connect(item,SIGNAL(newProfile(QVariant)),this,SLOT(newProfile(QVariant)));
    connect(item,SIGNAL(changeProfile(QVariant)),this,SLOT(changeProfile(QVariant)));
    connect(item,SIGNAL(deleteProfile(int)),this,SLOT(deleteProfile(int)));
    connect(item,SIGNAL(connectProfile(int)),this,SLOT(connectProfile(int)));
    connect(item,SIGNAL(playSong(QString)),mNetAccess,SLOT(playTrack(QString)));
    connect(item,SIGNAL(playFiles(QString)),mNetAccess,SLOT(playFiles(QString)));
    connect(item,SIGNAL(addSong(QString)),mNetAccess,SLOT(addTrackToPlaylist(QString)));
    connect(item,SIGNAL(addSongAfterCurrent(QString)),mNetAccess,SLOT(addTrackAfterCurrent(QString)));
    connect(item,SIGNAL(playPlaylistSongNext(int)),mNetAccess,SLOT(playTrackNext(int)));
    connect(item,SIGNAL(requestSavedPlaylist(QString)),mNetAccess,SLOT(getPlaylistTracks(QString)));
    connect(item,SIGNAL(addPlaylist(QString)),mNetAccess,SLOT(addPlaylist(QString)));
    connect(item,SIGNAL(playPlaylist(QString)),mNetAccess,SLOT(playPlaylist(QString)));
    connect(item,SIGNAL(setShuffle(bool)),mNetAccess,SLOT(setRandom(bool)));
    connect(item,SIGNAL(setRepeat(bool)),mNetAccess,SLOT(setRepeat(bool)));
    connect(item,SIGNAL(updateDB()),mNetAccess,SLOT(updateDB()));
    connect(item,SIGNAL(popfilemodelstack()),this,SLOT(fileStackPop()));
    connect(item,SIGNAL(cleanFileStack()),this,SLOT(cleanFileStack()));
    connect(&volDecTimer,SIGNAL(timeout()),this,SLOT(decVolume()));
    connect(&volIncTimer,SIGNAL(timeout()),this,SLOT(incVolume()));
    //connect(QApplication::instance(),SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));

    connect(this,SIGNAL(getFiles(QString)),mNetAccess,SLOT(getDirectory(QString)));
    connect(this,SIGNAL(setVolume(int)),mNetAccess,SLOT(setVolume(int)));
    connect(this,SIGNAL(requestConnect()),mNetAccess,SLOT(connectToHost()));
    connect(this,SIGNAL(requestDisconnect()),mNetAccess,SLOT(disconnectFromServer()));
//    connect(this,SIGNAL(serverProfilesUpdated()),item,SLOT(settingsModelUpdated()));
    connect(this,SIGNAL(setUpdateInterval(int)),mNetAccess,SLOT(setUpdateInterval(int)));

    connect(mNetAccess,SIGNAL(outputsReady(QList<QObject*>*)),this,SLOT(updateOutputsModel(QList<QObject*>*)));
    connect(item,SIGNAL(requestOutputs()),mNetAccess,SLOT(getOutputs()));
    connect(item,SIGNAL(enableOutput(int)),mNetAccess,SLOT(enableOutput(int)));
    connect(item,SIGNAL(disableOutput(int)),mNetAccess,SLOT(disableOutput(int)));

    connect(item,SIGNAL(requestSearch(QVariant)),mNetAccess,SLOT(searchTracks(QVariant)));
    connect(item,SIGNAL(addlastsearch()),this,SLOT(addlastsearchtoplaylist()));
    connect(this,SIGNAL(addURIToPlaylist(QString)),mNetAccess,SLOT(addTrackToPlaylist(QString)));

    //connect(this,SIGNAL(requestPlaylistClear()),item,SLOT(clearPlaylist()));
    connect(this,SIGNAL(filesModelReady()),item,SLOT(receiveFilesPage()));
    connect(this,SIGNAL(filePopCleared()),item,SLOT(popCleared()));

    connect(mQuickView,SIGNAL(focusObjectChanged(QObject*)),this,SLOT(focusChanged(QObject*)));


    connect(this,SIGNAL(requestArtistAlbumMap()),mNetAccess,SLOT(getArtistAlbumMap()));
    connect(this,SIGNAL(requestArtists()),mNetAccess,SLOT(getArtists()));

    connect(mNetAccess,SIGNAL(artistsAlbumsMapReady(QMap<MpdArtist*,QList<MpdAlbum*>*>*)),mImgDB,SLOT(fillDatabase(QMap<MpdArtist*,QList<MpdAlbum*>*>*)));
    connect(item,SIGNAL(clearAlbumList()),this,SLOT(clearAlbumList()));
    connect(item,SIGNAL(clearArtistList()),this,SLOT(clearArtistList()));
    connect(item,SIGNAL(clearTrackList()),this,SLOT(clearTrackList()));
    connect(item,SIGNAL(clearPlaylists()),this,SLOT(clearPlaylists()));

    connect(this,SIGNAL(connected(QVariant)),item,SLOT(slotConnected(QVariant)));
    connect(this,SIGNAL(disconnected()),item,SLOT(slotDisconnected()));

    connect(&mReconnectTimer,SIGNAL(timeout()),this,SLOT(reconnectServer()));

    connect(this,SIGNAL(requestCoverArt(MpdAlbum)),mImgDB,SLOT(requestCoverImage(MpdAlbum)));
    connect(mImgDB,SIGNAL(coverAlbumArtReady(QVariant)),item,SLOT(coverArtReceiver(QVariant)));

    connect(this,SIGNAL(requestCoverArtistArt(MpdArtist)),mImgDB,SLOT(requestCoverArtistImage(MpdArtist)));
    connect(mImgDB,SIGNAL(coverArtistArtReady(QVariant)),item,SLOT(coverArtistArtReceiver(QVariant)));

    connect(this,SIGNAL(requestArtistImageFill(QList<MpdArtist*>*)),mImgDB,SLOT(fillDatabase(QList<MpdArtist*>*)));

    connect(item,SIGNAL(bulkDownloadArtists()),this,SLOT(fillArtistImages()));
    connect(item,SIGNAL(bulkDownloadAlbums()),this,SLOT(fillAlbumImages()));

    connect(this, SIGNAL(requestDBStatistic()),mImgDB,SLOT(requestStatisticUpdate()));
    connect(mImgDB,SIGNAL(newStasticReady(DatabaseStatistic*)),this,SLOT(newDBStatisticReceiver(DatabaseStatistic*)));

    connect(item,SIGNAL(cleanupBlacklisted()),mImgDB,SLOT(cleanUPBlacklistedAlbums()));
    connect(item,SIGNAL(cleanupAlbums()),mImgDB,SLOT(cleanupAlbums()));
    connect(item,SIGNAL(cleanupArtists()),mImgDB,SLOT(cleanupArtists()));
    connect(item,SIGNAL(cleanupDB()),mImgDB,SLOT(cleanupDatabase()));

    connect(item,SIGNAL(requestAlbumInfo(QVariant)),mImgDB,SLOT(requestAlbumWikiInformation(QVariant)));
    connect(item,SIGNAL(requestArtistInfo(QString)),mImgDB,SLOT(requestArtistBioInformation(QString)));

    connect(mImgDB,SIGNAL(albumWikiInformationReady(QString)),this,SLOT(setAlbumWikiInfo(QString)));
    connect(mImgDB,SIGNAL(artistBioInformationReady(QString)),this,SLOT(setArtistBioInfo(QString)));

    // Received new Download size from database GUI settings
    connect(item,SIGNAL(newDownloadSize(int)),this,SLOT(receiveDownloadSize(int)));
    connect(this,SIGNAL(newDownloadSize(QString)),mImgDB,SLOT(setDownloadSize(QString)));

    // Receive GUI settings here
    connect(item,SIGNAL(newSettingKey(QVariant)),this,SLOT(receiveSettingKey(QVariant)));

    connect(item,SIGNAL(addSongToSaved(QVariant)),mNetAccess,SLOT(addTrackToSavedPlaylist(QVariant)));
    connect(item,SIGNAL(removeSongFromSaved(QVariant)),mNetAccess,SLOT(removeTrackFromSavedPlaylist(QVariant)));

    // Set downloading enabled variable to imagedatabase
    connect(this,SIGNAL(newDownloadEnabled(bool)),mImgDB,SLOT(setDownloadEnabled(bool)));

    connect(item,SIGNAL(wakeUpServer(int)),this,SLOT(wakeUpHost(int)));

    /* New status object connection */
    connect(mPlaybackStatus,SIGNAL(albumChanged()),this,SLOT(onNewAlbum()));
    connect(mPlaybackStatus,SIGNAL(artistChanged()),this,SLOT(onNewArtist()));

    /* new playlist model connects */
    connect(mNetAccess,SIGNAL(currentPlaylistReady(QList<MpdTrack*>*)),mPlaylist,SLOT(receiveNewTrackList(QList<MpdTrack*>*)));
    connect(mPlaybackStatus,SIGNAL(idChanged(quint32)),mPlaylist,SLOT(onTrackNoChanged(quint32)));
    connect(mPlaybackStatus,SIGNAL(playbackStatusChanged(MpdPlaybackState)),mPlaylist,SLOT(onPlaybackStateChanged(MpdPlaybackState)));

    /* new saved tracks model connects */
    connect(mNetAccess,SIGNAL(trackListReady(QList<MpdTrack*>*)),mOtherTracks,SLOT(receiveNewTrackList(QList<MpdTrack*>*)));
}

void Controller::setPassword(QString password)
{
    this->mPassword = password;
}

void Controller::setHostname(QString hostname)
{
    this->mHostname = hostname;
}


void Controller::setPort(int port)
{
    this->mPort = port;
}

void Controller::connectToServer()
{
    //netaccess->connectToHost(hostname,port,password);
    mNetAccess->setConnectParameters(mHostname,mPort,mPassword);
    emit requestConnect();
    //Try authentication

}

void Controller::requestCurrentPlaylist()
{
    //netaccess->getCurrentPlaylistTracks();
}


void Controller::requestAlbum(QVariant array)
{
    // New qt 5.4 qml->c++ qvariant cast
    if (array.userType() == qMetaTypeId<QJSValue>()) {
        array = qvariant_cast<QJSValue>(array).toVariant();
    }
    QStringList strings = array.toStringList();
    for(int i=0;i<strings.length();i++)
    {
    }
    mNetAccess->getAlbumTracks(strings.at(1),strings.at(0));
}



void Controller::connectedToServer()
{
    mReconnectTimer.stop();
    QString popupString = tr("Connected to: ") + mProfilename;
    mReconnectTimer.stop();
    mWasConnected = true;
    emit sendPopup(popupString);
    emit connected(mProfilename);

//    QUrl streamurl("http://"+mHostname + ":" + "8081");
//    //QUrl streamurl("http://uwstream2.somafm.com:2666");
//    //QUrl streamurl = QUrl::fromLocalFile("/usr/share/sounds/jolla-ringtones/stereo/jolla-ringtone.wav");
//    qDebug() << "start playback of url: " << streamurl.toString();

//    mStreamPlayer->setURL(streamurl);
//    mStreamPlayer->startPlayback();

    // emit requestArtistAlbumMap();
}

void Controller::disconnectedToServer()
{    emit sendPopup(tr("Disconnected from server"));
     emit disconnected();
     if(mApplicationActive) {
         mReconnectTimer.setSingleShot(true);
         mReconnectTimer.start();
     }
}

void Controller::onNewAlbum()
{
    if ( mPlaybackStatus->getPlaybackStatus() != MPD_STOP ) {
        // Request cover/artist art if song has changed
        MpdAlbum tmpAlbum(this,mPlaybackStatus->getAlbum(),mPlaybackStatus->getArtist());
        qDebug()  << "Requesting cover Image for currently playing album: " << tmpAlbum.getTitle() << tmpAlbum.getArtist();
        emit requestCoverArt(tmpAlbum);
    } else {
        // Clear cover/artist image by requesting empty images
        MpdAlbum tmpAlbum(this,"","");
        emit requestCoverArt(tmpAlbum);
    }
}

void Controller::onNewArtist()
{
    if ( mPlaybackStatus->getPlaybackStatus() != MPD_STOP ) {
        // Request cover/artist art if song has changed
        MpdAlbum tmpAlbum(this,mPlaybackStatus->getAlbum(),mPlaybackStatus->getArtist());
        qDebug()  << "Requesting cover Image for currently playing album: " << tmpAlbum.getTitle() << tmpAlbum.getArtist();
        emit requestCoverArt(tmpAlbum);

        MpdArtist tmpArtist(this,mPlaybackStatus->getArtist());
        qDebug() << "Requesting cover artist Image for currently playing title: " << tmpArtist.getName();
        emit requestCoverArtistArt(tmpArtist);
    } else {
        MpdArtist tmpArtist(this,"");
        emit requestCoverArtistArt(tmpArtist);
    }
}

void Controller::seek(int pos)
{
    mNetAccess->seekPosition(mCurrentSongID,pos);
}

void Controller::requestFilePage(QString path)
{
    emit getFiles(path);
}

void Controller::readSettings()
{
    if ( mServerProfiles ) {
        mQuickView->rootContext()->setContextProperty("serverList",0);
        delete(mServerProfiles);
        mServerProfiles = 0;
    }
    QList<ServerProfile*> *tmpList = new QList< ServerProfile*> ();
    QSettings settings;
    settings.beginGroup("server_properties");
    int size = settings.beginReadArray("profiles");
    QString hostname,password,name,macAddr;
    int port;
    bool autoconnect;
    for(int i = 0;i<size;i++)
    {
        settings.setArrayIndex(i);
        hostname = settings.value("hostname").toString();
        password = settings.value("password").toString();
        name = settings.value("profilename").toString();
        port = settings.value("port").toUInt();
        autoconnect = settings.value("default").toBool();
        macAddr = settings.value("macaddress").toString();
        tmpList->append(new ServerProfile(hostname,password,port,name,autoconnect,macAddr));
    }
    settings.endArray();
    settings.endGroup();

    settings.beginGroup("general_properties");
    int dlSize = settings.value("download_size",LASTFM_EXTRALARGE).toInt();
    mArtistViewSetting = settings.value("artist_view",0).toInt();
    mAlbumViewSetting = settings.value("album_view",0).toInt();
    mListImageSize = settings.value("list_image_size",0).toInt();
    mSectionsInSearch = settings.value("sections_in_search",1).toInt();
    mSectionsInPlaylist = settings.value("sections_in_playlist",1).toInt();
    mDownloadEnabled = settings.value("lastfm_download",1).toInt();
    mCoverInNowPlaying = settings.value("show_covernowplaying",1).toInt();
    mShowModeLandscape = settings.value("useShowView",1).toInt();

    emit newDownloadEnabled(mDownloadEnabled);

    mQuickView->rootContext()->setContextProperty("artistView", mArtistViewSetting);
    mQuickView->rootContext()->setContextProperty("albumView", mAlbumViewSetting);
    mQuickView->rootContext()->setContextProperty("listImageSize", mListImageSize);
    mQuickView->rootContext()->setContextProperty("sectionsInSearch", mSectionsInSearch);
    mQuickView->rootContext()->setContextProperty("sectionsInPlaylist", mSectionsInPlaylist);
    mQuickView->rootContext()->setContextProperty("lastfmEnabled", mDownloadEnabled);
    mQuickView->rootContext()->setContextProperty("showCoverNowPlaying", mCoverInNowPlaying);

    mQuickView->rootContext()->setContextProperty("useShowView", mShowModeLandscape);

    mQuickView->rootContext()->setContextProperty("downloadSize",dlSize);
    mDownloadSize = dlSize;
    emit newDownloadSize(getLastFMArtSize(mDownloadSize));
    settings.endGroup();
    mServerProfiles = new ServerProfileModel(tmpList,this);
    mQuickView->rootContext()->setContextProperty("serverList",mServerProfiles);
    emit serverProfilesUpdated();
    if(mServerProfiles->rowCount() ==0)
    {
        emit showWelcome();
    }
}

void Controller::writeSettings()
{
    QSettings settings;
    settings.clear();
    settings.beginGroup("server_properties");
    settings.beginWriteArray("profiles");
    for(int i=0;i<mServerProfiles->rowCount();i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("hostname",mServerProfiles->get(i)->getHostname());
        settings.setValue("password",mServerProfiles->get(i)->getPassword());
        settings.setValue("profilename",mServerProfiles->get(i)->getName());
        settings.setValue("port",mServerProfiles->get(i)->getPort());
        settings.setValue("default",mServerProfiles->get(i)->getAutoconnect());
        settings.setValue("macaddress",mServerProfiles->get(i)->getMACAddress());
    }
    settings.endArray();
    settings.endGroup();
    settings.beginGroup("general_properties");
    settings.setValue("download_size",mDownloadSize);
    settings.setValue("artist_view",mArtistViewSetting);
    settings.setValue("album_view",mAlbumViewSetting);
    settings.setValue("list_image_size",mListImageSize);
    settings.setValue("sections_in_search",mSectionsInSearch);
    settings.setValue("sections_in_playlist",mSectionsInPlaylist);
    settings.setValue("lastfm_download",mDownloadEnabled);
    settings.setValue("show_covernowplaying",mCoverInNowPlaying);
    settings.setValue("useShowView",mShowModeLandscape);
    settings.endGroup();
}

void Controller::quit()
{
    writeSettings();
    connect(this,SIGNAL(requestExit()),mNetAccess,SLOT(exitRequest()));
    connect(mNetAccess,SIGNAL(requestExit()),this,SLOT(exitRequest()));
    QTimer *exittimer = new QTimer();
    exittimer->setInterval(10000);
    exittimer->setSingleShot(true);
    connect(exittimer,SIGNAL(timeout()),this,SLOT(exitRequest()));
    emit requestExit();
    exittimer->start();
}

void Controller::exitRequest()
{
    mNetworkThread->exit(0);
    exit(0);
}

void Controller::newProfile(QVariant profile)
{
    // New qt 5.4 qml->c++ qvariant cast
    if (profile.userType() == qMetaTypeId<QJSValue>()) {
        profile = qvariant_cast<QJSValue>(profile).toVariant();
    }
    QStringList strings = profile.toStringList();
    QString hostname,password, profilename;
    hostname = strings[2];
    password = strings[3];
    profilename = strings[1];
    int port = strings.at(4).toInt();
    bool autoconnect;
    QString macAddr = strings.at(6);
    if(strings.at(5).toInt()==1) {
        //Check for other autoconnects
        for(int j = 0; j<mServerProfiles->rowCount();j++)
        {
            mServerProfiles->get(j)->setAutoconnect(false);
            mServerProfiles->notifyChanged(j);
        }
        autoconnect = true;
    }
    else{
        autoconnect = false;
    }
    ServerProfile *tempprofile = new ServerProfile(hostname,password,port,profilename,autoconnect,macAddr);
    QQmlEngine::setObjectOwnership(tempprofile,QQmlEngine::CppOwnership);
    mServerProfiles->append(tempprofile);
    emit serverProfilesUpdated();
    writeSettings();
}

void Controller::changeProfile(QVariant profile)
{
    // New qt 5.4 qml->c++ qvariant cast
    if (profile.userType() == qMetaTypeId<QJSValue>()) {
        profile = qvariant_cast<QJSValue>(profile).toVariant();
    }
    QStringList strings = profile.toStringList();
    int i = strings.at(0).toInt();
    mServerProfiles->get(i)->setName(strings[1]);
    mServerProfiles->get(i)->setHostname(strings[2]);
    mServerProfiles->get(i)->setPassword(strings[3]);
    mServerProfiles->get(i)->setPort(strings.at(4).toInt());
    mServerProfiles->get(i)->setMACAdress(strings[6]);
    mServerProfiles->notifyChanged(i);
    if(strings.at(5).toInt()==1) {
        //Check for other autoconnects
        for(int j = 0; j<mServerProfiles->rowCount();j++)
        {
            mServerProfiles->get(j)->setAutoconnect(false);
            mServerProfiles->notifyChanged(j);
        }
        mServerProfiles->get(i)->setAutoconnect(true);
    }
    else{
        mServerProfiles->get(i)->setAutoconnect(false);
    }
    mServerProfiles->notifyChanged(i);
    writeSettings();
}

void Controller::deleteProfile(int index)
{
    mServerProfiles->remove(index);
    writeSettings();
}

void Controller::connectProfile(int index)
{
    ServerProfile *profile = mServerProfiles->get(index);
    setHostname(profile->getHostname());
    setPort(profile->getPort());
    setPassword(profile->getPassword());
    mHostname = profile->getHostname();
    mPort =  profile->getPort();
    mPassword = profile->getPassword();
    mProfilename = profile->getName();
    mReconnectTimer.setInterval(5000);
    if ( mReconnectTimer.isActive() ) {
        mReconnectTimer.stop();
    }
    mQuickView->rootContext()->setContextProperty("profilename",QVariant::fromValue(QString(mProfilename)));
    connectToServer();
}

void Controller::incVolume()
{
    emit setVolume((mVolume+3>100 ? 100 : mVolume+3));
    mVolume =(mVolume+3>100 ? 100 : mVolume+3);
    QString popup = "Volume: "+ QString::number(mVolume)+"%";
    emit sendPopup(popup);
}

void Controller::decVolume()
{
    emit setVolume((mVolume-3<0 ? 0 : mVolume-3));
    mVolume = (mVolume-3<0 ? 0 : mVolume-3);
    QString popup = "Volume: "+ QString::number(mVolume)+"%";
    emit sendPopup(popup);

}


void Controller::focusChanged(QObject *now){
    if(now==0)
    {
        mApplicationActive = false;
        emit setUpdateInterval(5000);
    }
    else if (!mApplicationActive){
        mApplicationActive = true;
        if ( !mNetAccess->connected() ) {
            reconnectServer();
        }
        emit setUpdateInterval(1000);
    }
}


void Controller::fileStackPop()
{
    FileModel *model = mFileModels->pop();
    delete(model);
    //    if(!filemodels->empty())
    //    {
    //        viewer->rootContext()->setContextProperty("filesModel",QVariant::fromValue(*(filemodels->top())));
    //    }
    emit filePopCleared();
}

void Controller::cleanFileStack()
{
    QList<MpdFileEntry*> *list;
    while(!mFileModels->empty())
    {
        list = (QList<MpdFileEntry*>*)mFileModels->pop();
        for(int i=0;i<list->length();i++)
        {
            delete(list->at(i));
        }
        delete(list);
    }
    mQuickView->rootContext()->setContextProperty("searchedTracksModel",0);
    emit filePopCleared();
}

void Controller::addlastsearchtoplaylist()
{
        for(int i = 0;i<mOtherTracks->rowCount();i++)
        {
            emit addURIToPlaylist(mOtherTracks->get(i)->getFileUri());
        }
}

void Controller::clearAlbumList()
{
    mQuickView->rootContext()->setContextProperty("albumsModel",0);
    if(mOldAlbumModel!=0)
    {
        delete(mOldAlbumModel);
        mOldAlbumModel = 0;
    }
}

void Controller::clearArtistList()
{
    mQuickView->rootContext()->setContextProperty("artistsModel",0);
    if(mOldArtistModel!=0)
    {
        delete(mOldArtistModel);
        mOldArtistModel = 0;
    }
}

void Controller::clearTrackList()
{
    mOtherTracks->receiveNewTrackList(0);
}

void Controller::clearPlaylists()
{
    mQuickView->rootContext()->setContextProperty("savedPlaylistsModel",0);
    if(mSavedPlaylists)
    {
        delete(mSavedPlaylists);
        mSavedPlaylists = 0;
    }
}

void Controller::reconnectServer()
{
    if( mWasConnected && !mNetAccess->connected() ) {
        // Just one attempt. otherwise we hang in loop if server is not reachable anymore
        mWasConnected = false;
        // Reconnect last profile
        connectToServer();
    }
}

void Controller::fillArtistImages()
{
    // Disconnect artit list signal (prevent appearing of artist lists page to UI)
    disconnect(mNetAccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(updateArtistsModel(QList<QObject*>*)));
    connect(mNetAccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(fillArtistImages(QList<QObject*>*)));

    qDebug() << "Requested artist list for image bulk downloader";
    emit requestArtists();
}

void Controller::fillArtistImages(QList<QObject *> *artistList)
{
    // Reconnect signal
    disconnect(mNetAccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(fillArtistImages(QList<QObject*>*)));
    connect(mNetAccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(updateArtistsModel(QList<QObject*>*)));
    qDebug() << "Received artist list for image bulk downloader";

    emit requestArtistImageFill((QList<MpdArtist*>*)artistList);
}

void Controller::newDBStatisticReceiver(DatabaseStatistic *statistic)
{
    mQuickView->rootContext()->setContextProperty("dbStatistic",statistic);
    if ( mDBStatistic ) {
        delete mDBStatistic;
    }
    mDBStatistic = statistic;
}

void Controller::fillAlbumImages()
{
    qDebug() << "Bulk download of albums requested";
    emit requestArtistAlbumMap();
}

void Controller::setArtistBioInfo(QString info)
{
    mQuickView->rootContext()->setContextProperty("artistInfoText",info);
}

void Controller::setAlbumWikiInfo(QString info)
{
    mQuickView->rootContext()->setContextProperty("albumInfoText",info);
}

void Controller::receiveDownloadSize(int size)
{
    mDownloadSize = size;
    mQuickView->rootContext()->setContextProperty("downloadSize",size);
    emit newDownloadSize(getLastFMArtSize(size));
    writeSettings();
}

void Controller::receiveSettingKey(QVariant setting)
{
    // New qt 5.4 qml->c++ qvariant cast
    if (setting.userType() == qMetaTypeId<QJSValue>()) {
        setting = qvariant_cast<QJSValue>(setting).toVariant();
    }
    QStringList settings = setting.toStringList();
    if ( settings.length() == 2 ) {
        if ( settings.at(0) == "albumView" ) {
            mAlbumViewSetting = settings.at(1).toInt();
            mQuickView->rootContext()->setContextProperty("albumView", mAlbumViewSetting);
        } else if ( settings.at(0) == "artistView" ) {
            mArtistViewSetting = settings.at(1).toInt();
            mQuickView->rootContext()->setContextProperty("artistView", mArtistViewSetting);
        } else if ( settings.at(0) == "listImageSize" ) {
            mListImageSize = settings.at(1).toInt();
            mQuickView->rootContext()->setContextProperty("listImageSize", mListImageSize);
        } else if ( settings.at(0) == "sectionsInSearch" ) {
            mSectionsInSearch = settings.at(1).toInt();
            mQuickView->rootContext()->setContextProperty("sectionsInSearch", mSectionsInSearch);
        } else if ( settings.at(0) == "sectionsInPlaylist" ) {
            mSectionsInPlaylist = settings.at(1).toInt();
            mQuickView->rootContext()->setContextProperty("sectionsInPlaylist", mSectionsInPlaylist);
        } else if ( settings.at(0) == "lastfmEnabled" ) {
            mDownloadEnabled = settings.at(1).toInt();
            mQuickView->rootContext()->setContextProperty("lastfmEnabled", mDownloadEnabled);
            emit newDownloadEnabled(mDownloadEnabled);
        } else if ( settings.at(0) == "showCoverNowPlaying" ) {
            mCoverInNowPlaying = settings.at(1).toInt();
            mQuickView->rootContext()->setContextProperty("showCoverNowPlaying", mCoverInNowPlaying);
        } else if ( settings.at(0) == "showModeLandscape" ) {
            mShowModeLandscape = settings.at(1).toInt();
            mQuickView->rootContext()->setContextProperty("useShowView", mShowModeLandscape);
        }

    }
    writeSettings();
}

QString Controller::getLastFMArtSize(int index)
{
    switch (index)  {
    case 0: {
        return "small";
        break;
    }
    case 1: {
        return "medium";
        break;
    }
    case 2: {
        return "large";
        break;
    }
    case 3: {
        return "extralarge";
        break;
    }
    case 4: {
        return "mega";
        break;
    }
    }
    return LASTFMDEFAULTSIZE;
}


void Controller::trimCache()
{
//    mQuickView->engine()->clearComponentCache();
//    mQuickView->engine()->collectGarbage();
}

void Controller::wakeUpHost(int index)
{
    if ( mServerProfiles == NULL || mServerProfiles->rowCount() < index || mServerProfiles->get(index)->getMACAddress()=="") {
        return;
    }

    QUdpSocket udpSocket;
    QByteArray dataGram;
    dataGram.append(QByteArray::fromHex("ffffffffffff"));
    for ( int i = 0; i < 16; i++) {
        dataGram.append(QByteArray::fromHex(mServerProfiles->get(index)->getMACAddress().toLatin1()));
    }

    qint64 bytesSend = udpSocket.writeDatagram(dataGram,QHostAddress::Broadcast,9);
    qDebug() << "Send WoL: " << bytesSend << " bytes, content: " << dataGram.toHex() << endl;
}
