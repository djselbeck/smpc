import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

// FIXME to harbour.smpc.components.whatever import
import "components"



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

    // Signals for database requests to networkaccess
    //Variant in format [artistname,albumname]
    signal addAlbum(variant album);
    signal addArtist(string artist);
    signal addFiles(string files);
    signal addSong(string uri);
    signal addPlaylist(string name);
    signal playPlaylist(string name);
    signal playAlbum(variant album);
    signal playArtist(string artist);
    signal playFiles(string uri);
    //appends song to playlist
    signal playSong(string uri);
    // Adds all tracks from last search result to playlist
    signal addlastsearch();
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
    signal requestSearch(variant request);
    // Request an MPD database update (remote mpd db)
    signal updateDB();


    // Signals controller that stack of filemodels can be wiped
    signal popfilemodelstack();
    signal cleanFileStack();

    // Controller model clear signals (memory cleanup)
    signal clearAlbumList();
    signal clearArtistList();
    signal clearTrackList();
    signal clearPlaylistTracks();
    signal clearPlaylists();

    // MPD Output controls
    signal enableOutput(int nr);
    signal disableOutput(int nr);

    // Control signals
    signal play();
    signal next();
    signal prev();
    signal stop();
    signal seek(int position);
    signal setRepeat(bool rep);
    signal setShuffle(bool shfl);

    //Playlist signals
    signal savePlaylist(string name);
    signal deletePlaylist();
    signal deleteSavedPlaylist(string name);
    signal playPlaylistTrack(int index);
    signal deletePlaylistTrack(int index);

    // Changes server profile or creates new one
    signal changeProfile(variant profile);
    signal newProfile(variant profile);
    signal deleteProfile(int index);
    // Requests controller to connect to MPD host
    signal connectProfile(int index);

    // Database stuff (local-metadata DB)
    signal bulkDownloadArtists();
    signal bulkDownloadAlbums();
    signal cleanupBlacklisted();
    signal cleanupArtists();
    signal cleanupAlbums();
    signal cleanupDB();

    // Wiki/Biography information requests for sqlDB
    signal requestArtistInfo(string artist);
    signal requestAlbumInfo(variant album);

    // Signals change in download-size preference
    signal newDownloadSize(int size)

    // Propertys of application
    property string hostname;

    property int port;
    property string password;
    property Page currentsongpage;
    property Page playlistpage;
    property Page serverList;
    property Page artistspage;
    property Page albumspage;
    property int listfontsize : 12;
    property int liststretch : 20;
    property int lastsongid : -1;
    property string playbuttoniconsource : "image://theme/icon-m-play";
    property string playbuttoniconsourcecover : "image://theme/icon-cover-play";
    property string volumebuttoniconsource;
    property string lastpath;
    property string artistname;
    property string albumname;
    property string playlistname;
    property string coverimageurl;
    property string artistimageurl;
    property string profilename;
    property bool repeat;
    property bool shuffle;
    property bool quitbtnenabled;
    property bool connected;
    property bool playing : false;
    property bool stopped : false;
    property bool fastscrollenabled : false;

    property real listPadding : Theme.paddingLarge

    property bool volumeChanging:false

    // current song information
    property string mTitle
    property string mArtist
    property string mAlbum
    property int mVolume
    property bool mRepeat
    property bool mShuffle
    property bool mDebugEnabled


    // JS-functions
    //TODO separation

    function slotConnected(profile)
    {
        profilename = profile;
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
        inputBlock.enabled = true
    }

    function ready()
    {
        busyIndicator.running = false;
        inputBlock.enabled = false
    }

    function settingsModelUpdated()
    {
        console.debug("Got new server list");
        serverList.listmodel = settingsModel;
    }

    function updateCurrentPlaying(list)
    {
        mTitle = list[0];
        mAlbum = list[1];
        mArtist = list[2];
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
        if(!volumeChanging){
            mVolume = list[7];
        }
        mRepeat = (list[8]=="0" ?  false:true);
        mShuffle = (list[9]=="0" ?  false:true);
        currentsongpage.nr = (list[10]==0? "":list[10]);
        currentsongpage.uri = list[11];
        if(list[12]!=lastsongid)
        {
            playlistpage.songid = list[12];
            lastsongid = list[12];
        }
        if(stopped) {
            coverimageurl = "";
            artistimageurl = "";
            mArtist=""
            mAlbum=""
            mTitle=""
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
        pageStack.push(Qt.resolvedUrl("pages/database/PlaylistTracksPage.qml"),{listmodel:savedPlaylistModel,playlistname:playlistname});
    }

    function updateSavedPlaylistsModel()
    {
        pageStack.push(Qt.resolvedUrl("pages/database/SavedPlaylistsPage.qml"),{listmodel:savedPlaylistsModel});
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
        // playlistModelVar = playlistModel;
        console.debug("received new playlist and set model");
    }


    function updateAlbumsModel(){
        pageStack.push(Qt.resolvedUrl("pages/database/AlbumListPage.qml"),{listmodel:albumsModel,artistname:artistname});
    }

    function updateArtistModel(){
        pageStack.push(Qt.resolvedUrl("pages/database/ArtistListPage.qml"),{listmodel:artistsModel});
    }

    function updateAlbumModel()
    {
        //pageStack.push(Qt.resolvedUrl("pages/database/AlbumTracksPage.qml"),{artistname:artistname,albumname:albumname,listmodel:albumTracksModel});
    }

    function updateOutputsModel()
    {
        pageStack.push(Qt.resolvedUrl("pages/settings/OutputsPage.qml"),{listmodel:outputsModel});
    }

    function albumTrackClicked(title,album,artist,lengthformatted,uri,year,tracknr)
    {
        pageStack.push(Qt.resolvedUrl("components/SongDialog.qml"),{title:title,album:album,artist:artist,filename:uri,lengthtext:lengthformatted,date:year,nr:tracknr});
    }

    function receiveFilesPage()
    {
        pageStack.push("pages/database/FileBrowserPage.qml",{listmodel: filesModel,filepath :lastpath});
        fastscrollenabled = true;
    }

    function popCleared() {
        console.debug("POP cleared");
        fastscrollenabled = true;
    }

    // Nicely formats length values to string
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

    function coverArtReceiver(url)
    {
        coverimageurl = url;
    }

    function coverArtistArtReceiver(url)
    {
        artistimageurl = url;
    }

    // Create some permanent pages which should reside in memory for all the time
    Component.onCompleted:  {
        var component = Qt.createComponent("pages/settings/ServerListPage.qml");
        var object = component.createObject(mainWindow);
        mainWindow.serverList = object;
        component = Qt.createComponent("pages/database/CurrentPlaylistPage.qml");
        object = component.createObject(mainWindow);
        mainWindow.playlistpage = object;
        pageStack.pushAttached(playlistpage);
        component = Qt.createComponent("pages/database/CurrentSong.qml");
        object = component.createObject(mainWindow);
        currentsongpage = object;
    }

    // Notifies user about ongoing action in netaccess
    BusyIndicator
    {
        id:busyIndicator
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
    }

    // Prevents clicking if active request is running
    MouseArea
    {
        id: inputBlock
        anchors.fill: parent
        preventStealing: true
        enabled: false
    }


    ControlPanel {
        id: quickControlPanel
    }
    //bottomMargin: (orientation===Orientation.Portrait ? quickControlPanel.visibleSize : 0 )


    initialPage: Qt.resolvedUrl("pages/MainPage.qml")
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
}
