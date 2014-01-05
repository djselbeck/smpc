#include "controller.h"
Controller::Controller(QObject *parent) : QObject(parent)
{

}

Controller::Controller(QQuickView *viewer,QObject *parent) : QObject(parent),viewer(viewer),password(""),hostname(""),port(6600)
{
    mImgDB = new ImageDatabase();
    mQMLImgProvider = new QMLImageProvider(mImgDB);

    netaccess = new NetworkAccess(0);
    netaccess->setUpdateInterval(1000);
    oldnetthread = netaccess->thread();
    networkthread = new QThreadEx(this);
    dbThread = new QThread(this);
    mImgDB->moveToThread(dbThread);
    netaccess->moveToThread(networkthread);
    networkthread->start();
    dbThread->start();
    currentsongid=0;
    playlistversion = 0;
    playlist = 0;
    artistlist = 0;
    outputs = 0;
    albumlist = 0;
    artistmodelold = 0;
    albumsmodelold = 0;
    albumTracks = 0;
    playlistTracks = 0;
    searchedtracks = 0;
    lastplaybackstate = NetworkAccess::STOP;
    connectSignals();
    readSettings();
    qmlRegisterType<MpdArtist>();
    qmlRegisterType<MpdAlbum>();
    qmlRegisterType<ServerProfile>();
    qRegisterMetaType<MpdAlbum>("MpdAlbum");
    volIncTimer.setInterval(250);
    volDecTimer.setInterval(250);
    mLastProfileIndex = -1;
    filemodels = new QStack<FileModel*>();
    viewer->rootContext()->setContextProperty("versionstring",QVariant::fromValue(QString(VERSION)));
    viewer->rootContext()->setContextProperty("coverstring","");
    viewer->engine()->addImageProvider("imagedbprovider",mQMLImgProvider);
    netaccess->setQmlThread(viewer->thread());
    //Start auto connect
    for(int i = 0;i<serverprofiles->length();i++)
    {
        if(serverprofiles->at(i)->getAutoconnect())
        {
            connectProfile(i);
        }
    }
    mDBStatistic = 0;
    mApplicationActive = true;


    emit requestDBStatistic();
}

void Controller::updatePlaylistModel(QList<QObject*>* list)
{
    emit requestPlaylistClear();
    if(playlist==0){
        currentsongid=0;
    } else{
        delete(playlist);
        playlist = 0;
    }
    currentsongid = -1;
    playlist = new PlaylistModel((QList<MpdTrack*>*)list,this);

    viewer->rootContext()->setContextProperty("playlistModel",playlist);
    emit playlistUpdated();
}

void Controller::updateFilesModel(QList<QObject*>* list)
{
    if(list->length()>0)
    {
        FileModel *model = new FileModel((QList<MpdFileEntry*>*)list,this);
        QQmlEngine::setObjectOwnership(model,QQmlEngine::CppOwnership);
        viewer->rootContext()->setContextProperty("filesModel",model);
        filemodels->push(model);
        emit filesModelReady();
    }

}

void Controller::updateSavedPlaylistsModel(QStringList *list)
{
    viewer->rootContext()->setContextProperty("savedPlaylistsModel",QVariant::fromValue(*list));
    emit savedPlaylistsReady();

}

void Controller::updateSavedPlaylistModel(QList<QObject*>* list)
{
    if ( playlistTracks ) {
        for( int i = 0; i < playlistTracks->length();i++)
        {
            delete(playlistTracks->at(i));
        }
        delete(playlistTracks);
        playlistTracks = 0;
    }
    playlistTracks = (QList<MpdTrack*>*)list;
    viewer->rootContext()->setContextProperty("savedPlaylistModel",QVariant::fromValue(*list));
    emit savedPlaylistReady();
}

void Controller::updateArtistsModel(QList<QObject*>* list)
{
    if(artistmodelold!=0)
    {
        delete(artistmodelold);
        artistmodelold = 0;
    }
    //ArtistModel *model = new ArtistModel((QList<MpdTrack*>*)list,this);
//    artistlist = (QList<MpdArtist*>*)list;
    ArtistModel *model = new ArtistModel((QList<MpdArtist*>*)list,mImgDB,this);
    QQmlEngine::setObjectOwnership(model,QQmlEngine::CppOwnership);
    artistmodelold = model;
    viewer->rootContext()->setContextProperty("artistsModel",model);
    emit artistsReady();
}

//void Controller::updateArtistAlbumsModel(QList<QObject*>* list)
//{
//    viewer->rootContext()->setContextProperty("albumsModel",QVariant::fromValue(*list));
//    emit artistAlbumsReady();
//}

void Controller::updateAlbumsModel(QList<QObject*>* list)
{
    if(albumsmodelold!=0)
    {
        delete(albumsmodelold);
        albumsmodelold = 0;
    }
    AlbumModel *model = new AlbumModel((QList<MpdAlbum*>*)list,mImgDB,this);
    QQmlEngine::setObjectOwnership(model,QQmlEngine::CppOwnership);
    albumsmodelold = model;

    viewer->rootContext()->setContextProperty("albumsModel",model);
    emit albumsReady();
}

void Controller::updateOutputsModel(QList<QObject*>* list)
{
    if(outputs!=0)
    {
        delete(outputs);
    }
    outputs = (QList<MPDOutput*>*)list;

    viewer->rootContext()->setContextProperty("outputsModel",QVariant::fromValue(*list));
    emit outputsReady();
}


void Controller::updateAlbumTracksModel(QList<QObject*>* list)
{
    if ( albumTracks ) {
        for( int i = 0; i < albumTracks->length();i++)
        {
            delete(albumTracks->at(i));
        }
        delete(albumTracks);
        albumTracks = 0;
    }
    albumTracks = (QList<MpdTrack*>*)list;
    viewer->rootContext()->setContextProperty("albumTracksModel",QVariant::fromValue(*list));
    emit albumTracksReady();
}

void Controller::updateSearchedTracks(QList<QObject*>* list)
{
    if(searchedtracks!=0)
    {
        delete (searchedtracks);
        searchedtracks = 0;
    }
    viewer->rootContext()->setContextProperty("searchedTracksModel",QVariant::fromValue(*list));
    searchedtracks = (QList<MpdTrack*>*)list;
    emit searchedTracksReady();
}

void Controller::connectSignals()
{
    QObject *item = (QObject *)viewer->rootObject();
    qRegisterMetaType<status_struct>("status_struct");
    qRegisterMetaType<QList<MpdTrack*>*>("QList<MpdTrack*>*");
    qRegisterMetaType<QList<MpdAlbum*>*>("QList<MpdAlbum*>*");
    qRegisterMetaType<QList<MpdArtist*>*>("QList<MpdArtist*>*");
    qRegisterMetaType<QList<MpdFileEntry*>*>("QList<MpdFileEntry*>*");
    connect(item,SIGNAL(setHostname(QString)),this,SLOT(setHostname(QString)));
    connect(item,SIGNAL(setPassword(QString)),this,SLOT(setPassword(QString)));
    connect(item,SIGNAL(setPort(int)),this,SLOT(setPort(int)));
    connect(item,SIGNAL(connectToServer()),this,SLOT(connectToServer()));
    connect(item,SIGNAL(requestCurrentPlaylist()),netaccess,SLOT(getCurrentPlaylistTracks()));
    connect(item,SIGNAL(requestArtists()),netaccess,SLOT(getArtists()));
    connect(item,SIGNAL(requestArtistAlbums(QString)),netaccess,SLOT(getArtistsAlbums(QString)));
    connect(item,SIGNAL(requestAlbums()),netaccess,SLOT(getAlbums()));
    connect(item,SIGNAL(requestFilesPage(QString)),this,SLOT(requestFilePage(QString)));
  //  connect(item,SIGNAL(requestCurrentPlaylist()),this,SLOT(requestCurrentPlaylist()));
    connect(item,SIGNAL(playPlaylistTrack(int)),netaccess,SLOT(playTrackByNumber(int)));
    connect(item,SIGNAL(deletePlaylistTrack(int)),netaccess,SLOT(deleteTrackByNumer(int)));
    connect(item,SIGNAL(requestAlbum(QVariant)),netaccess,SLOT(getAlbumTracks(QVariant)));
    connect(item,SIGNAL(stop()),netaccess,SLOT(stop()));
    connect(item,SIGNAL(play()),netaccess,SLOT(pause()));
    connect(item,SIGNAL(next()),netaccess,SLOT(next()));
    connect(item,SIGNAL(prev()),netaccess,SLOT(previous()));
    connect(item,SIGNAL(deletePlaylist()),netaccess,SLOT(clearPlaylist()));
    connect(item,SIGNAL(addAlbum(QVariant)),netaccess,SLOT(addArtistAlbumToPlaylist(QVariant)));
    connect(item,SIGNAL(playAlbum(QVariant)),netaccess,SLOT(playArtistAlbum(QVariant)));
    connect(item,SIGNAL(addFiles(QString)),netaccess,SLOT(addTrackToPlaylist(QString)));
    connect(item,SIGNAL(seek(int)),netaccess,SLOT(seek(int)));
    connect(item,SIGNAL(setVolume(int)),netaccess,SLOT(setVolume(int)));
    connect(item,SIGNAL(addArtist(QString)),netaccess,SLOT(addArtist(QString)));
    connect(item,SIGNAL(playArtist(QString)),netaccess,SLOT(playArtist(QString)));
    connect(item,SIGNAL(quit()),this,SLOT(quit()));
    connect(item,SIGNAL(savePlaylist(QString)),netaccess,SLOT(savePlaylist(QString)));
    connect(item,SIGNAL(deleteSavedPlaylist(QString)),netaccess,SLOT(deletePlaylist(QString)));
    connect(item,SIGNAL(requestSavedPlaylists()),netaccess,SLOT(getSavedPlaylists()));
    connect(netaccess,SIGNAL(savedPlaylistsReady(QStringList*)),this,SLOT(updateSavedPlaylistsModel(QStringList*)));
    connect(netaccess,SIGNAL(savedplaylistTracksReady(QList<QObject*>*)),this,SLOT(updateSavedPlaylistModel(QList<QObject*>*)));
    connect(netaccess,SIGNAL(currentPlayListReady(QList<QObject*>*)),this,SLOT(updatePlaylistModel(QList<QObject*>*)));
    connect(netaccess,SIGNAL(albumsReady(QList<QObject*>*)),this,SLOT(updateAlbumsModel(QList<QObject*>*)));
    connect(netaccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(updateArtistsModel(QList<QObject*>*)));
    connect(netaccess,SIGNAL(artistAlbumsReady(QList<QObject*>*)),this,SLOT(updateAlbumsModel(QList<QObject*>*)));
    connect(netaccess,SIGNAL(albumTracksReady(QList<QObject*>*)),this,SLOT(updateAlbumTracksModel(QList<QObject*>*)));
    connect(netaccess,SIGNAL(filesReady(QList<QObject*>*)),this,SLOT(updateFilesModel(QList<QObject*>*)));
    connect(netaccess,SIGNAL(connectionestablished()),this,SLOT(connectedToServer()));
    connect(netaccess,SIGNAL(disconnected()),this,SLOT(disconnectedToServer()));
    connect(netaccess,SIGNAL(statusUpdate(status_struct)),this,SLOT(updateStatus(status_struct)));
    connect(netaccess,SIGNAL(busy()),item,SLOT(busy()));
    connect(netaccess,SIGNAL(ready()),item,SLOT(ready()));
    connect(item,SIGNAL(newProfile(QVariant)),this,SLOT(newProfile(QVariant)));
    connect(item,SIGNAL(changeProfile(QVariant)),this,SLOT(changeProfile(QVariant)));
    connect(item,SIGNAL(deleteProfile(int)),this,SLOT(deleteProfile(int)));
    connect(item,SIGNAL(connectProfile(int)),this,SLOT(connectProfile(int)));
    connect(item,SIGNAL(playSong(QString)),netaccess,SLOT(playTrack(QString)));
    connect(item,SIGNAL(playFiles(QString)),netaccess,SLOT(playFiles(QString)));
    connect(item,SIGNAL(addSong(QString)),netaccess,SLOT(addTrackToPlaylist(QString)));
    connect(item,SIGNAL(requestSavedPlaylist(QString)),netaccess,SLOT(getPlaylistTracks(QString)));
    connect(item,SIGNAL(addPlaylist(QString)),netaccess,SLOT(addPlaylist(QString)));
    connect(item,SIGNAL(playPlaylist(QString)),netaccess,SLOT(playPlaylist(QString)));
    connect(item,SIGNAL(setShuffle(bool)),netaccess,SLOT(setRandom(bool)));
    connect(item,SIGNAL(setRepeat(bool)),netaccess,SLOT(setRepeat(bool)));
    connect(item,SIGNAL(updateDB()),netaccess,SLOT(updateDB()));
    connect(item,SIGNAL(popfilemodelstack()),this,SLOT(fileStackPop()));
    connect(item,SIGNAL(cleanFileStack()),this,SLOT(cleanFileStack()));
    connect(&volDecTimer,SIGNAL(timeout()),this,SLOT(decVolume()));
    connect(&volIncTimer,SIGNAL(timeout()),this,SLOT(incVolume()));
    //connect(QApplication::instance(),SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));
    connect(this,SIGNAL(albumsReady()),item,SLOT(updateAlbumsModel()));
    connect(this,SIGNAL(artistsReady()),item,SLOT(updateArtistModel()));
    connect(this,SIGNAL(albumTracksReady()),item,SLOT(updateAlbumModel()));
    connect(this,SIGNAL(savedPlaylistsReady()),item,SLOT(updateSavedPlaylistsModel()));
    connect(this,SIGNAL(savedPlaylistReady()),item,SLOT(updateSavedPlaylistModel()));
    connect(this,SIGNAL(sendStatus(QVariant)),item,SLOT(updateCurrentPlaying(QVariant)));
    connect(this,SIGNAL(playlistUpdated()),item,SLOT(updatePlaylist()));
    connect(this,SIGNAL(getFiles(QString)),netaccess,SLOT(getDirectory(QString)));
    connect(this,SIGNAL(setVolume(int)),netaccess,SLOT(setVolume(int)));
    connect(this,SIGNAL(requestConnect()),netaccess,SLOT(connectToHost()));
    connect(this,SIGNAL(requestDisconnect()),netaccess,SLOT(disconnect()));
    connect(this,SIGNAL(serverProfilesUpdated()),item,SLOT(settingsModelUpdated()));
    connect(this,SIGNAL(setUpdateInterval(int)),netaccess,SLOT(setUpdateInterval(int)));

    connect(netaccess,SIGNAL(outputsReady(QList<QObject*>*)),this,SLOT(updateOutputsModel(QList<QObject*>*)));
    connect(this,SIGNAL(outputsReady()),item,SLOT(updateOutputsModel()));
    connect(item,SIGNAL(requestOutputs()),netaccess,SLOT(getOutputs()));
    connect(item,SIGNAL(enableOutput(int)),netaccess,SLOT(enableOutput(int)));
    connect(item,SIGNAL(disableOutput(int)),netaccess,SLOT(disableOutput(int)));

    connect(item,SIGNAL(requestSearch(QVariant)),netaccess,SLOT(searchTracks(QVariant)));
    connect(netaccess,SIGNAL(searchedTracksReady(QList<QObject*>*)),this,SLOT(updateSearchedTracks(QList<QObject*>*)));
    connect(this,SIGNAL(searchedTracksReady()),item,SLOT(updateSearchedModel()));
    connect(item,SIGNAL(addlastsearch()),this,SLOT(addlastsearchtoplaylist()));
    connect(this,SIGNAL(addURIToPlaylist(QString)),netaccess,SLOT(addTrackToPlaylist(QString)));

    connect(this,SIGNAL(requestPlaylistClear()),item,SLOT(clearPlaylist()));
    connect(this,SIGNAL(filesModelReady()),item,SLOT(receiveFilesPage()));
    connect(this,SIGNAL(filePopCleared()),item,SLOT(popCleared()));

    connect(viewer,SIGNAL(focusObjectChanged(QObject*)),this,SLOT(focusChanged(QObject*)));


    connect(this,SIGNAL(requestArtistAlbumMap()),netaccess,SLOT(getArtistAlbumMap()));
    connect(netaccess,SIGNAL(artistsAlbumsMapReady(QMap<MpdArtist*,QList<MpdAlbum*>*>*)),mImgDB,SLOT(fillDatabase(QMap<MpdArtist*,QList<MpdAlbum*>*>*)));
    connect(item,SIGNAL(clearAlbumList()),this,SLOT(clearAlbumList()));
    connect(item,SIGNAL(clearArtistList()),this,SLOT(clearArtistList()));
    connect(item,SIGNAL(clearTrackList()),this,SLOT(clearTrackList()));
    connect(item,SIGNAL(clearPlaylistTracks()),this,SLOT(clearPlaylistList()));

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
}

void Controller::setPassword(QString password)
{
    this->password = password;
}

void Controller::setHostname(QString hostname)
{
    this->hostname = hostname;
}


void Controller::setPort(int port)
{
    this->port = port;
}

void Controller::connectToServer()
{
    //netaccess->connectToHost(hostname,port,password);
    netaccess->setConnectParameters(hostname,port,password);
    emit requestConnect();
    //Try authentication

}

void Controller::requestCurrentPlaylist()
{
    //netaccess->getCurrentPlaylistTracks();
}

void Controller::requestAlbums()
{
    netaccess->getAlbums();
}

void Controller::requestArtists()
{
    netaccess->getArtists();
}

void Controller::requestArtistAlbums(QString artist)
{
    netaccess->getArtistsAlbums(artist);
}

void Controller::requestAlbum(QVariant array)
{
    QStringList strings = array.toStringList();
    for(int i=0;i<strings.length();i++)
    {
    }
    netaccess->getAlbumTracks(strings.at(1),strings.at(0));
}



void Controller::connectedToServer()
{
    mReconnectTimer.stop();
    QString popupString = tr("Connected to: ") + profilename;
    mReconnectTimer.stop();
    emit sendPopup(popupString);
    emit connected(profilename);
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

void Controller::updateStatus(status_struct status)
{

    if(currentsongid != status.id)
    {
        if(status.playing==NetworkAccess::PLAYING) {
            QString popup = status.title+"\n"+status.album+"\n"+status.artist;
            emit sendPopup(popup);
        }
        if(playlist!=0&&playlist->rowCount()>status.id&&playlist->rowCount()>currentsongid
                &&status.id>=0&&currentsongid>=0){
            playlist->setPlaying(currentsongid,false);
            playlist->setPlaying(status.id,true);
//            playlist->get(currentsongid)->setPlaying(false);
//            playlist->get(status.id)->setPlaying(true);

        }
        if(currentsongid==-1&&(playlist!=0&&playlist->rowCount()>status.id&&playlist->rowCount()>currentsongid
                               &&status.id>=0))
        {
            playlist->setPlaying(status.id,true);
        }

        // Check for cover and set URL if ready
        MpdAlbum tmpAlbum(this,status.album,status.artist);
        qDebug()  << "Requesting cover Image for currently playing album: " << tmpAlbum.getTitle() << tmpAlbum.getArtist();
        emit requestCoverArt(tmpAlbum);

        MpdArtist tmpArtist(this,status.artist);
        qDebug() << "Requesting cover artist Image for currently playing title: " << tmpArtist.getName();
        emit requestCoverArtistArt(tmpArtist);

    }
    if(lastplaybackstate!=status.playing)
    {
        if(status.playing==NetworkAccess::STOP&&playlist!=0&&currentsongid>=0&&currentsongid<playlist->rowCount())
        {
            playlist->setPlaying(currentsongid,false);
//            playlist->get(currentsongid)->setPlaying(false);
        }
    }
    lastplaybackstate = status.playing;
    currentsongid = status.id;
    if(playlist==0)
        currentsongid = -1;
    QStringList strings;
    strings.append(status.title);
    strings.append(status.album);
    strings.append(status.artist);
    strings.append(QString::number(status.currentpositiontime));
    strings.append(QString::number(status.length));
    strings.append(QString::number(status.bitrate));
    switch (status.playing) {
    case NetworkAccess::PLAYING:
    {
        strings.append("playing");
        break;
    }
    case NetworkAccess::PAUSE:
    {
        strings.append("pause");
        break;
    }
    case NetworkAccess::STOP:
    {
        strings.append("stop");
        break;
    }
    default: strings.append("stop");
    }
    strings.append(QString::number(status.volume));
    strings.append(QString::number(status.repeat));
    strings.append(QString::number(status.shuffle));
    strings.append(QString::number(status.tracknr));
    strings.append(status.fileuri);
    strings.append(QString::number(status.id));
    strings.append(QString::number(status.samplerate));
    strings.append(QString::number(status.bitdepth));
    strings.append(QString::number(status.channelcount));
    volume = status.volume;
    emit sendStatus(strings);
}

void Controller::seek(int pos)
{
    netaccess->seekPosition(currentsongid,pos);
}

void Controller::requestFilePage(QString path)
{
    QObject *item = (QObject *)viewer->rootObject();
    emit getFiles(path);
}

void Controller::readSettings()
{
    serverprofiles = new QList< ServerProfile*> ();
    QSettings settings;
    settings.beginGroup("server_properties");
    int size = settings.beginReadArray("profiles");
    QString hostname,password,name;
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
        serverprofiles->append(new ServerProfile(hostname,password,port,name,autoconnect));
    }
    settings.endArray();
    settings.endGroup();
    viewer->rootContext()->setContextProperty("settingsModel",QVariant::fromValue(*(QList<QObject*>*)serverprofiles));
    emit serverProfilesUpdated();
    if(serverprofiles->length()==0)
    {
        emit showWelcome();
    }
}

void Controller::writeSettings()
{
    QSettings settings;
        settings.beginGroup("server_properties");
        settings.beginWriteArray("profiles");
        for(int i=0;i<serverprofiles->length();i++)
        {
            settings.setArrayIndex(i);
            settings.setValue("hostname",serverprofiles->at(i)->getHostname());
            settings.setValue("password",serverprofiles->at(i)->getPassword());
            settings.setValue("profilename",serverprofiles->at(i)->getName());
            settings.setValue("port",serverprofiles->at(i)->getPort());
            settings.setValue("default",serverprofiles->at(i)->getAutoconnect());
        }
        settings.endArray();
        settings.endGroup();
}

void Controller::quit()
{
    writeSettings();
    connect(this,SIGNAL(requestExit()),netaccess,SLOT(exitRequest()));
    connect(netaccess,SIGNAL(requestExit()),this,SLOT(exitRequest()));
    QTimer *exittimer = new QTimer();
    exittimer->setInterval(10000);
    exittimer->setSingleShot(true);
    connect(exittimer,SIGNAL(timeout()),this,SLOT(exitRequest()));
    emit requestExit();
    exittimer->start();
}

void Controller::exitRequest()
{
    networkthread->exit(0);
    exit(0);
}

//void Controller::newProfile()
//{
//    serverprofiles->append(new ServerProfile("","",6600,"Profile_"+QString::number(serverprofiles->length()),false));
//    viewer->rootContext()->setContextProperty("settingsModel",QVariant::fromValue(*(QList<QObject*>*)serverprofiles));
//    emit serverProfilesUpdated();
//}

void Controller::newProfile(QVariant profile)
{
    QStringList strings = profile.toStringList();
    QString hostname,password, profilename;
    hostname = strings[2];
    password = strings[3];
    profilename = strings[1];
    int port = strings.at(4).toInt();
    bool autoconnect;
    if(strings.at(5).toInt()==1) {
        //Check for other autoconnects
        for(int j = 0; j<serverprofiles->length();j++)
        {
            serverprofiles->at(j)->setAutoconnect(false);
        }
        autoconnect = true;
    }
    else{
        autoconnect = false;
    }
    ServerProfile *tempprofile = new ServerProfile(hostname,password,port,profilename,autoconnect);
    serverprofiles->append(tempprofile);
    viewer->rootContext()->setContextProperty("settingsModel",QVariant::fromValue(*(QList<QObject*>*)serverprofiles));
    emit serverProfilesUpdated();
    writeSettings();
}

void Controller::changeProfile(QVariant profile)
{
    QStringList strings = profile.toStringList();
    int i = strings.at(0).toInt();
    serverprofiles->at(i)->setName(strings[1]);
    serverprofiles->at(i)->setHostname(strings[2]);
    serverprofiles->at(i)->setPassword(strings[3]);
    serverprofiles->at(i)->setPort(strings.at(4).toInt());
    if(strings.at(5).toInt()==1) {
    //Check for other autoconnects
        for(int j = 0; j<serverprofiles->length();j++)
        {
            serverprofiles->at(j)->setAutoconnect(false);
        }
        serverprofiles->at(i)->setAutoconnect(true);
    }
    else{
        serverprofiles->at(i)->setAutoconnect(false);
    }
    emit serverProfilesUpdated();
    writeSettings();
}

void Controller::deleteProfile(int index)
{
    serverprofiles->removeAt(index);
    viewer->rootContext()->setContextProperty("settingsModel",QVariant::fromValue(*(QList<QObject*>*)serverprofiles));
    emit serverProfilesUpdated();
}

void Controller::connectProfile(int index)
{
    ServerProfile *profile = serverprofiles->at(index);
    setHostname(profile->getHostname());
    setPort(profile->getPort());
    setPassword(profile->getPassword());
    hostname = profile->getHostname();
    port =  profile->getPort();
    password = profile->getPassword();
    profilename = profile->getName();
    mLastProfileIndex = index;
    mReconnectTimer.setInterval(5000);
    viewer->rootContext()->setContextProperty("profilename",QVariant::fromValue(QString(profilename)));
    if(netaccess->connected())
    {
        emit requestDisconnect();
    }
    connectToServer();
}

void Controller::incVolume()
{
    emit setVolume((volume+3>100 ? 100 : volume+3));
    volume =(volume+3>100 ? 100 : volume+3);
    QString popup = "Volume: "+ QString::number(volume)+"%";
    emit sendPopup(popup);
}

void Controller::decVolume()
{
    emit setVolume((volume-3<0 ? 0 : volume-3));
    volume = (volume-3<0 ? 0 : volume-3);
    QString popup = "Volume: "+ QString::number(volume)+"%";
    emit sendPopup(popup);

}
void Controller::mediaKeyHandle(int key)
{
//    if(key == MediaKeysObserver::EVolDecKey)
//        decVolume();
//    if(key == MediaKeysObserver::EVolIncKey)
//        incVolume();

}

void Controller::mediaKeyPressed(int key)
{
//    if(key == MediaKeysObserver::EVolDecKey)
//        volDecTimer.start();
//    if(key == MediaKeysObserver::EVolIncKey)
//        volIncTimer.start();
}

void Controller::mediaKeyReleased(int key)
{
//    if(key == MediaKeysObserver::EVolDecKey&&volDecTimer.isActive())
//        volDecTimer.stop();
//    if(key == MediaKeysObserver::EVolIncKey&&volIncTimer.isActive())
//        volIncTimer.stop();
}


void Controller::focusChanged(QObject *now){
    if(now==0)
    {
        mApplicationActive = false;
        emit setUpdateInterval(5000);
    }
    else if (!mApplicationActive){
        mApplicationActive = true;
        if ( !netaccess->connected() && (mLastProfileIndex!=-1) ) {
           reconnectServer();
        }
        emit setUpdateInterval(1000);
    }
}


void Controller::fileStackPop()
{
    FileModel *model = filemodels->pop();
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
    while(!filemodels->empty())
    {
            list = (QList<MpdFileEntry*>*)filemodels->pop();
            for(int i=0;i<list->length();i++)
            {
                delete(list->at(i));
            }
            delete(list);
    }
    if(searchedtracks!=0){
        delete(searchedtracks);
        searchedtracks = 0;
    }
    emit filePopCleared();
}

void Controller::addlastsearchtoplaylist()
{
    if ( searchedtracks ) {
        for(int i = 0;i<searchedtracks->length();i++)
        {
            emit addURIToPlaylist(searchedtracks->at(i)->getFileUri());
        }
    }
}

void Controller::clearAlbumList()
{
    if(albumsmodelold!=0)
    {
        delete(albumsmodelold);
        albumsmodelold = 0;
    }
}

void Controller::clearArtistList()
{
    if(artistmodelold!=0)
    {
        delete(artistmodelold);
        artistmodelold = 0;
    }
}

void Controller::clearTrackList()
{
    if ( albumTracks ) {
        for( int i = 0; i < albumTracks->length();i++)
        {
            delete(albumTracks->at(i));
        }
        delete(albumTracks);
        albumTracks = 0;
    }
}

void Controller::clearPlaylistList()
{
    if ( playlistTracks ) {
        for( int i = 0; i < playlistTracks->length();i++)
        {
            delete(playlistTracks->at(i));
        }
        delete(playlistTracks);
        playlistTracks = 0;
    }
}

void Controller::reconnectServer()
{
    if( (mLastProfileIndex>=0) && (mLastProfileIndex<serverprofiles->length()
            && !netaccess->connected() ) ) {
        connectProfile(mLastProfileIndex);
    }
}

void Controller::fillArtistImages()
{
    // Disconnect artit list signal (prevent appearing of artist lists page to UI)
    disconnect(netaccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(updateArtistsModel(QList<QObject*>*)));
    connect(netaccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(fillArtistImages(QList<QObject*>*)));

    qDebug() << "Requested artist list for image bulk downloader";
    emit requestArtists();
}

void Controller::fillArtistImages(QList<QObject *> *artistList)
{
    // Reconnect signal
    disconnect(netaccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(fillArtistImages(QList<QObject*>*)));
    connect(netaccess,SIGNAL(artistsReady(QList<QObject*>*)),this,SLOT(updateArtistsModel(QList<QObject*>*)));
    qDebug() << "Received artist list for image bulk downloader";

    emit requestArtistImageFill((QList<MpdArtist*>*)artistList);
}

void Controller::newDBStatisticReceiver(DatabaseStatistic *statistic)
{
    viewer->rootContext()->setContextProperty("dbStatistic",statistic);
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


