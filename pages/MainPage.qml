import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    SilicaListView {
        anchors.fill: parent
        contentWidth: width
        header: PageHeader {
            title: "SMPC"
        }
        model: mainMenuModel
        delegate: BackgroundItem {
            Label {
                clip: true
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                    leftMargin: listPadding
                    rightMargin: listPadding
                }
                text: name
            }
            onClicked: {
                parseClickedMainMenu(ident)
            }
        }
    }

    Component.onCompleted: {
        mainMenuModel.append({"name":qsTr("song information"), "ident":"currentsong"})
        mainMenuModel.append({"name":qsTr("artists"), "ident":"artists"})
        mainMenuModel.append({"name":qsTr("albums"), "ident":"albums"})
        mainMenuModel.append({"name":qsTr("files"), "ident":"files"})
        mainMenuModel.append({"name":qsTr("playlist"), "ident":"playlist"})
        mainMenuModel.append({"name":qsTr("search"), "ident":"search"})
        mainMenuModel.append({"name":qsTr("connect"), "ident":"connectto"})
        mainMenuModel.append({"name":qsTr("settings"), "ident":"settings"})
    }

    ListModel {
        id: mainMenuModel

    }

    Timer {
        id: showCurrentSongTimer
        interval: 15000
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
