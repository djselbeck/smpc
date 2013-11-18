import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    SilicaListView {
        anchors.fill: parent
        anchors.margins: Theme.paddingMedium
        contentWidth: width
        header: PageHeader {
            title: "SMPC"
        }
        model: mainMenuModel
        delegate: BackgroundItem {
            Label {
                anchors.centerIn: parent
                text: name
            }
            onClicked: {
                parseClickedMainMenu(ident)
            }
        }
    }

    ListModel {
        id: mainMenuModel
        ListElement {
            name: "song information"
            ident: "currentsong"
            icon: "icons/music.svg"
        }
        ListElement {
            name: "artists"
            ident: "artists"
            icon: "icons/contacts.svg"
        }
        ListElement {
            name: "albums"
            ident: "albums"
            icon: "icons/music_album.svg"
        }
        ListElement {
            name: "files"
            ident: "files"
            icon: "icons/music_file.svg"
        }
        ListElement {
            name: "playlist"
            ident: "playlist"
            icon: "icons/playlistlist.svg"
        }
        ListElement {
            name: "search"
            ident: "search"
            icon: "icons/search.svg"
        }
        ListElement {
            name: "connect"
            ident: "connectto"
            icon: "icons/connectivity.svg"
        }
        ListElement {
            name: "settings"
            ident: "settings"
            icon: "icons/settings.svg"
        }
    }

    Timer {
        id: showCurrentSongTimer
        interval: 10000
        repeat: false
        onTriggered: {
            parseClickedMainMenu("currentsong")
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            showCurrentSongTimer.start()
        } else if (status == PageStatus.Deactivating) {
            if (showCurrentSongTimer.running) {
                showCurrentSongTimer.stop()
            }
        }
    }

    function parseClickedMainMenu(ident) {
        if (ident == "playlist") {
            if (connected)
                pageStack.push(playlistpage)
        } else if (ident == "settings") {
            pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
        } else if (ident == "currentsong") {
            if (connected)
                pageStack.push(currentsongpage)
//                        if(connected)
//                            pageStack.push(Qt.resolvedUrl("CurrentSong.qml"));
        } else if (ident == "albums") {
            artistname = ""
            if (connected)
                requestAlbums()
        } else if (ident == "artists") {
            if (connected)
                requestArtists()
        } else if (ident == "files") {
            if (connected)
                filesClicked("/")
        } else if (ident == "connectto") {
            pageStack.push(Qt.resolvedUrl("ConnectServerPage.qml"), {
                               listmodel: settingsModel
                           })
        } else if (ident == "about") {
            aboutdialog.visible = true
            aboutdialog.version = versionstring
            aboutdialog.open()
        } else if (ident == "updatedb") {
            updateDB()
        } else if (ident == "search") {
            pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
        }
    }
}
