import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"


ApplicationWindow
{
    id: mainWindow

    // Signals for c++<->qml communication
    signal setHostname(string hostname);
    signal setPort(int port);
    signal setPassword(string password);
    signal setVolume(int volume);
    signal setCurrentArtist(string artist);
    signal connectToServer();
    //Variant in format [artistname,albumname]
    signal addAlbum(variant album);
    signal addArtist(string artist);
    signal addFiles(string files);
    signal addSong(string uri);
    signal addPlaylist(string name);
    signal playPlaylist(string name);
    signal playAlbum(variant album);
    signal playArtist(string artist);


    signal requestSavedPlaylists();
    signal requestSavedPlaylist(string name);
    signal requestAlbums();
    signal requestAlbum(variant album);
    signal requestArtists();
    signal requestArtistAlbums(string artist);
    signal requestFilesPage(string files);
    signal requestFilesModel(string files);
    signal requestCurrentPlaylist();
    signal requestOutputs();
    signal popfilemodelstack();
    signal cleanFileStack();
    signal enableOutput(int nr);
    signal disableOutput(int nr);
    signal requestSearch(variant request);

    signal clearAlbumList();
    signal clearArtistList();
    signal clearTrackList();
    signal clearPlaylistTracks();

    // Control signals
    signal play();
    signal next();
    signal prev();
    signal stop();
    signal seek(int position);
    signal setRepeat(bool rep);
    signal setShuffle(bool shfl);
    signal updateDB();

    //Playlist signals
    signal savePlaylist(string name);
    signal deletePlaylist();
    signal deleteSavedPlaylist(string name);
    signal playPlaylistTrack(int index);
    signal deletePlaylistTrack(int index);
    signal changeProfile(variant profile);
    signal newProfile(variant profile);
    signal deleteProfile(int index);
    signal connectProfile(int index);
    //appends song to playlist
    signal playSong(string uri);
    //Clears playlist before adding
    signal playFiles(string uri);
    signal addlastsearch();

    signal quit();

    // Propertys of application
    property string hostname;

    // ONLY FOR USE WITH JOLLAMPC
    property string lastfmapikey : "1c728df8f626849518971eaae29e01a1";

    property int port;
    property string password;
    property Page currentsongpage;
    property Page playlistpage;
    property Page serverList;
    property Page artistspage;
    property Page albumspage;
    property var playlistModelVar;
    property int listfontsize:12;
    property int liststretch:20;
    property int lastsongid:-1;
    property string playbuttoniconsource;
    property string playbuttoniconsourcecover;
    property string volumebuttoniconsource;
    property string lastpath;
    property string artistname;
    property string albumname;
    property string playlistname;
    property string coverimageurl;
    property string artistimageurl;
    property bool repeat;
    property bool shuffle;
    property bool quitbtnenabled;
    property bool connected;
    property bool playing:false;
    property bool stopped:false;
    property bool fastscrollenabled:false;

    property real listPadding: Theme.paddingLarge



    // JS-functions
    //TODO separation

    function slotConnected()
    {
        connected = true;
    }

    function slotDisconnected()
    {
        connected = false;
        profilename = "";
        playing = false;
    }

    function busy()
    {
        busyIndicator.running = true;
    }

    function ready()
    {
        busyIndicator.running = false;
    }

    function settingsModelUpdated()
    {
        console.debug("Got new server list");
        serverList.listmodel = settingsModel;
    }

    function updateCurrentPlaying(list)
    {
        currentsongpage.title = list[0];
        currentsongpage.album = list[1];
        currentsongpage.artist = list[2];
        if(currentsongpage.pospressed==false) {
            currentsongpage.position = list[3];
        }
        currentsongpage.length = list[4];
        //currentsongpage.lengthtextcurrent = formatLength(list[3]);
        currentsongpage.lengthtextcomplete = list[4]==0 ? "": formatLength(list[4]);
        currentsongpage.bitrate = list[5]+"kbps";
        playbuttoniconsource = (list[6]=="playing") ? "image://theme/icon-m-pause" : "image://theme/icon-m-play";
        playbuttoniconsourcecover = (list[6]=="playing") ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play";
        playing = (list[6]=="playing") ? true : false;
        stopped = (list[6]=="stop") ? true : false;
        if(currentsongpage.volumepressed==false){
            currentsongpage.volume = list[7];
        }
        currentsongpage.repeat = (list[8]=="0" ?  false:true);
        currentsongpage.shuffle = (list[9]=="0" ?  false:true);
        currentsongpage.nr = (list[10]==0? "":list[10]);
        currentsongpage.uri = list[11];
        if(list[12]!=lastsongid)
        {
            playlistpage.songid = list[12];
            lastsongid = list[12];
            currentsongpage.makeLastFMRequestURL();
        }
        if(stopped) {
            coverimageurl = "";
            artistimageurl = "";
        }

        currentsongpage.audioproperties = list[13]+ "Hz "+ list[14] + "Bits " + list[15]+ "Channels";
    }

    function savedPlaylistClicked(modelData)
    {
        playlistname = modelData;
        requestSavedPlaylist(modelData);
    }

    function updateSavedPlaylistModel()
    {
        pageStack.push(Qt.resolvedUrl("pages/PlaylistTracksPage.qml"),{listmodel:savedPlaylistModel,playlistname:playlistname});
    }

    function updateSavedPlaylistsModel()
    {
        pageStack.push(Qt.resolvedUrl("pages/SavedPlaylistsPage.qml"),{listmodel:savedPlaylistsModel});
    }

    function filesClicked(path)
    {
        lastpath = path;
        requestFilesPage(path);
    }

    function updatePlaylist()
    {   
        //  blockinteraction.enabled=false;
        console.debug("setting new playlist");
        playlistpage.songid = -1;
        playlistModelVar = playlistModel;
        console.debug("received new playlist and set model");
    }


    function clearPlaylist()
    {
        playlistpage.listmodel = 0;
        console.debug("playlist model cleared");
    }

    function updateAlbumsModel(){
        pageStack.push(Qt.resolvedUrl("pages/AlbumListPage.qml"),{listmodel:albumsModel,artistname:artistname});
    }

    function updateArtistModel(){
        pageStack.push(Qt.resolvedUrl("pages/ArtistListPage.qml"),{listmodel:artistsModel});
    }

    function updateAlbumModel()
    {
        pageStack.push(Qt.resolvedUrl("pages/AlbumTracksPage.qml"),{artistname:artistname,albumname:albumname,listmodel:albumTracksModel});
    }

    function updateOutputsModel()
    {
        pageStack.push(Qt.resolvedUrl("pages/OutputsPage.qml"),{listmodel:outputsModel});
    }

    function albumTrackClicked(title,album,artist,lengthformatted,uri,year,tracknr)
    {
        pageStack.push(Qt.resolvedUrl("pages/SongPage.qml"),{title:title,album:album,artist:artist,filename:uri,lengthtext:lengthformatted,date:year,nr:tracknr});
    }

    function receiveFilesPage()
    {
        pageStack.push("pages/FileBrowserPage.qml",{listmodel: filesModel,filepath :lastpath});
        fastscrollenabled = true;
    }

    function popCleared() {
        console.debug("POP cleared");
        fastscrollenabled = true;
    }

    function formatLength(length)
    {
        var temphours = Math.floor(length/3600);
        var min = 0;
        var sec = 0;
        var temp="";
        if(temphours>1)
        {
            min=(length-(3600*temphours))/60;
        }
        else{
            min=Math.floor(length/60);
        }
        sec = length-temphours*3600-min*60;
        if(temphours===0)
        {
            temp=((min<10?"0":"")+min)+":"+(sec<10?"0":"")+(sec);
        }
        else
        {
            temp=((temphours<10?"0":"")+temphours)+":"+((min<10?"0":"")+min)+":"+(sec<10?"0":"")+(sec);
        }
        return temp;
    }

    function albumClicked(artist,albumstring)
    {
        requestAlbum([artist,albumstring]);
        artistname = artist;
        albumname = albumstring;
    }


    function updateSearchedModel()
    {
        pageStack.currentPage.listmodel = searchedTracksModel;
    }

    function artistClicked(item)
    {
        mainWindow.artistname = item;
        requestArtistAlbums(item);
    }

    Component.onCompleted:  {
        var component = Qt.createComponent("pages/ServerListPage.qml");
        var object = component.createObject(mainWindow);
        mainWindow.serverList = object;
        console.debug("ServerListPage created");
        component = Qt.createComponent("pages/CurrentPlaylistPage.qml");
        object = component.createObject(mainWindow);
        mainWindow.playlistpage = object;
        console.debug("PlaylistPage created");
        component = Qt.createComponent("pages/CurrentSong.qml");
        object = component.createObject(mainWindow);
        currentsongpage = object;
//        console.debug("currentsong Page created");
    }
    BusyIndicator
    {
        id:busyIndicator
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
    }

    initialPage: Qt.resolvedUrl("pages/MainPage.qml") //MainPage { }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

}
