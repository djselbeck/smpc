import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: mainPage
    allowedOrientations: bothOrientation
    PageHeader {
        id: mainHeader
        title: "SMPC"
        anchors{
            top: parent.top
            right: parent.right
            left: parent.left
        }
    }
    Label {
        id: connectedLabel
        anchors{
            top: mainHeader.bottom
            right: parent.right
            left: parent.left
        }
        horizontalAlignment: Text.AlignHCenter
        color: Theme.highlightColor
        text: connected ? qsTr("connected to")
                          + ": " + profilename : qsTr("disconnected")
    }
    SilicaListView {
        id: mainList
        clip: true
        anchors {
            top: connectedLabel.bottom
            bottom: parent.bottom
//            bottomMargin: quickControlPanel.visibleSize
            right: parent.right
            left: parent.left
        }

        contentWidth: width


        model: mainMenuModel
        delegate: ListItem {
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
        mainMenuModel.append({
                                 name: qsTr("playlist"),
                                 ident: "playlist"
                             })
//        mainMenuModel.append({
//                                 name: qsTr("song information"),
//                                 ident: "currentsong"
//                             })
        mainMenuModel.append({
                                 name: qsTr("artists"),
                                 ident: "artists"
                             })
        mainMenuModel.append({
                                 name: qsTr("albums"),
                                 ident: "albums"
                             })
        mainMenuModel.append({
                                 name: qsTr("files"),
                                 ident: "files"
                             })
        mainMenuModel.append({
                                 name: qsTr("search"),
                                 ident: "search"
                             })
        mainMenuModel.append({
                                 name: qsTr("connect"),
                                 ident: "connectto"
                             })
        mainMenuModel.append({
                                 name: qsTr("settings"),
                                 ident: "settings"
                             })
    }

    ListModel {
        id: mainMenuModel
    }

    Timer {
        id: showCurrentSongTimer
        interval: 15000
        repeat: false
        onTriggered: {
            if (connected){
                pageStack.push(Qt.resolvedUrl("database/CurrentPlaylistPage.qml"))
            }
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("database/CurrentPlaylistPage.qml"));
            showCurrentSongTimer.start()
        } else if (status === PageStatus.Deactivating) {
            if (showCurrentSongTimer.running) {
                showCurrentSongTimer.stop()
            }
        }
        else if ( status === PageStatus.Activating ) {


            //pageStack.pushAttached(currentsongpage);
        }
    }

    function parseClickedMainMenu(ident) {
        if (ident == "playlist") {
            if (connected){
                pageStack.navigateForward(PageStackAction.Animated)
            }
        } else if (ident == "settings") {
            pageStack.push(Qt.resolvedUrl("settings/SettingsPage.qml"))
        } else if (ident == "currentsong") {
            if (connected)
                pageStack.push(currentsongpage)
            //                        if(connected)
            //                            pageStack.push(Qt.resolvedUrl("CurrentSong.qml"));
        } else if (ident == "albums") {
            artistname = ""
            if (connected) {
                requestAlbums()
                pageStack.push(Qt.resolvedUrl("database/AlbumListPage.qml"),{artistname:artistname});
            }
        } else if (ident == "artists") {
            if (connected) {
                requestArtists()
                pageStack.push(Qt.resolvedUrl("database/ArtistListPage.qml"));
            }
        } else if (ident == "files") {
            if (connected)
                filesClicked("/")
        } else if (ident == "connectto") {
            pageStack.push(Qt.resolvedUrl("settings/ConnectServerPage.qml"))
        } else if (ident == "about") {
            aboutdialog.visible = true
            aboutdialog.version = versionstring
            aboutdialog.open()
        } else if (ident == "updatedb") {
            updateDB()
        } else if (ident == "search") {
            pageStack.push(Qt.resolvedUrl("database/SearchPage.qml"))
        }
    }
}
