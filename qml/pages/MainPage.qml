import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: mainPage
    allowedOrientations: Orientation.All
    PageHeader {
        id: mainHeader
        title: "SMPC"
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
        }
    }
    Label {
        id: connectedLabel
        anchors {
            top: mainHeader.bottom
            right: parent.right
            left: parent.left
        }
        horizontalAlignment: Text.AlignHCenter
        color: Theme.highlightColor
        text: connected ? qsTr("connected to") + ": " + profilename : qsTr(
                              "disconnected")
    }
    SilicaFlickable {
        anchors {
            top: connectedLabel.bottom
            bottom: parent.bottom
            right: parent.right
            left: parent.left
        }
        contentHeight: mainGrid.height
        clip: true
        Item {
            height: mainGrid.height
            width: parent.width
            Grid {
                id: mainGrid

                columns: Screen.sizeCategory
                         >= Screen.Large ? 3 : (orientation === Orientation.Landscape
                                                || orientation
                                                === Orientation.LandscapeInverted) ? 4 : 2
                anchors.horizontalCenter: parent.horizontalCenter
                Repeater {
                    model: mainMenuModel
                    delegate: Component {
                        BackgroundItem {
                            id: gridItem
                            width: Theme.itemSizeHuge
                            height: Theme.itemSizeHuge
                            Rectangle {
                                anchors.fill: parent
                                anchors.margins: Theme.paddingSmall
                                color: Theme.rgba(
                                           Theme.highlightBackgroundColor,
                                           Theme.highlightBackgroundOpacity)
                            }
                            Column {
                                anchors.centerIn: parent
                                Image {
                                    id: itemIcon
                                    source: icon
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                                Label {
                                    id: itemLabel
                                    anchors {
                                        horizontalCenter: parent.horizontalCenter
                                    }
                                    font.pixelSize: Theme.fontSizeMedium
                                    width: gridItem.width - (2 * Theme.paddingSmall)
                                    horizontalAlignment: "AlignHCenter"
                                     scale: paintedWidth > width ? (width / paintedWidth) : 1
                               /*     transform: [
                                        Scale {
                                            id: scale
                                            xScale: yScale
                                            yScale: itemLabel.width > (gridItem.width - (2 * Theme.paddingSmall)) ? (gridItem.width - (2 * Theme.paddingSmall)) / itemLabel.width : 1
                                        },
                                        Translate {
                                                        x: scale.xScale != 1 ? ((gridItem.width - (2 * Theme.paddingSmall))-itemLabel.width*scale.xScale)/2 : 0 ;
                                                        y: scale.yScale != 1 ? ((gridItem.height - (2 * Theme.paddingSmall))-itemLabel.height*scale.yScale)/2 : 0;}
                                    ]
*/
                                    text: name
                                }
                            }

                            onClicked: {
                                parseClickedMainMenu(ident)
                            }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        mainMenuModel.append({
                                 name: qsTr("playlist"),
                                 ident: "playlist",
                                 icon: "image://theme/icon-m-document"
                             })
        mainMenuModel.append({
                                 name: qsTr("artists"),
                                 ident: "artists",
                                 icon: "image://theme/icon-m-mic"
                             })
        mainMenuModel.append({
                                 name: qsTr("albums"),
                                 ident: "albums",
                                 icon: "image://theme/icon-m-music"
                             })
        mainMenuModel.append({
                                 name: qsTr("files"),
                                 ident: "files",
                                 icon: "image://theme/icon-m-folder"
                             })
        mainMenuModel.append({
                                 name: qsTr("search"),
                                 ident: "search",
                                 icon: "image://theme/icon-m-search"
                             })
        mainMenuModel.append({
                                 name: qsTr("connect"),
                                 ident: "connectto",
                                 icon: "image://theme/icon-m-computer"
                             })
        mainMenuModel.append({
                                 name: qsTr("settings"),
                                 ident: "settings",
                                 icon: "image://theme/icon-m-developer-mode"
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
            if (connected) {
                pageStack.navigateForward(PageStackAction.Animated)
            }
        }
    }

    function parseClickedMainMenu(ident) {
        if (ident == "playlist") {
            if (connected) {
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
                pageStack.push(Qt.resolvedUrl("database/AlbumListPage.qml"), {
                                   artistname: artistname
                               })
            }
        } else if (ident == "artists") {
            if (connected) {
                requestArtists()
                pageStack.push(Qt.resolvedUrl("database/ArtistListPage.qml"))
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

    onStatusChanged: {
        if (status === PageStatus.Active) {
            //            pageStack.pushAttached(Qt.resolvedUrl("database/CurrentPlaylistPage.qml"));
            if (mPlaylistPage == undefined) {
                /* Check if running on large device and load corresponding page */
                if (Screen.sizeCategory >= Screen.Large) {
                    var playlistComponent = Qt.createComponent(
                                Qt.resolvedUrl(
                                    "database/CurrentPlaylistPage_large.qml"))
                    var playlistPage = playlistComponent.createObject(
                                mainWindow)
                } else {
                    var playlistComponent = Qt.createComponent(
                                Qt.resolvedUrl(
                                    "database/CurrentPlaylistPage.qml"))
                    var playlistPage = playlistComponent.createObject(
                                mainWindow)
                }
                mPlaylistPage = playlistPage
            }

            pageStack.pushAttached(mPlaylistPage)
            showCurrentSongTimer.start()
        } else if (status === PageStatus.Deactivating) {
            if (showCurrentSongTimer.running) {
                showCurrentSongTimer.stop()
            }
        }
    }
}
