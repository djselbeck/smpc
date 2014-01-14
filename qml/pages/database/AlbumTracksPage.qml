import QtQuick 2.1

import Sailfish.Silica 1.0
import "../../components"

Page {
    id: albumTracksPage
    property alias listmodel: albumTracksListView.model
    property string albumname
    property string artistname
    property int lastIndex:0

    SilicaListView {
        id: albumTracksListView
        anchors.fill: parent
        anchors.bottomMargin: quickControlPanel.visibleSize
        contentWidth: width
        clip: true
        header: Item {
            height: headerColumn.height
            width: parent.width
            Column {
                id: headerColumn
                height: header.height + imageRow.height
                width: parent.width
                PageHeader {
                    id: header
                    title: albumname
                }
                Row {
                    id: imageRow
                    width: parent.width
                    height: width / 2
                    Image {
                        id: artistImage
                        width: parent.width / 2
                        height: imageRow.height
                        cache: false
                        asynchronous: true
                        sourceSize.width: width
                        sourceSize.height: height
                        source: artistname
                                == "" ? "image://imagedbprovider/artistfromalbum/"
                                        + albumname : "image://imagedbprovider/artist/" + artistname
                        fillMode: Image.PreserveAspectCrop
                        onStatusChanged: {
                            if ( status == Image.Error && artistImage.status == Image.Error ) {
                                // Disable image and set imageRow height to 0
                                imageRow.height = 0;
                            }
                        }
                    }
                    Image {
                        id: albumImage
                        width: parent.width / 2
                        height: imageRow.height
                        cache: false
                        asynchronous: true
                        sourceSize.width: width
                        sourceSize.height: height
                        source: artistname == "" ? "image://imagedbprovider/album/"
                                                   + albumname : "image://imagedbprovider/album/"
                                                   + artistname + "/" + albumname
                        fillMode: Image.PreserveAspectCrop
                        onStatusChanged: {
                            if ( status == Image.Error && artistImage.status == Image.Error ) {
                                // Disable image and set imageRow height to 0
                                imageRow.height = 0;
                            }
                        }
                    }
                }
            }
            OpacityRampEffect {
                sourceItem: headerColumn
                direction: OpacityRamp.TopToBottom
            }
        }
        PullDownMenu {
            MenuItem {
                text: qsTr("add album")
                onClicked: {
                    addAlbum([artistname, albumname])
                }
            }
            MenuItem {
                text: qsTr("play album")
                onClicked: {
                    playAlbum([artistname, albumname])
                }
            }
        }
        delegate: ListItem {
            menu: contextMenu
            property int workaroundHeight: mainColumn.height
            height: workaroundHeight
            Column {
                id: mainColumn
                clip: true
                height: (titleRow.height + artistLabel.height
                         >= Theme.itemSizeSmall ? titleRow.height
                                                  + artistLabel.height : Theme.itemSizeSmall)
                anchors {
                    right: parent.right
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    leftMargin: listPadding
                    rightMargin: listPadding
                }

                Row {
                    id: titleRow
                    Label {
                        text: (index + 1) + ". "
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    Label {
                        clip: true
                        wrapMode: Text.WrapAnywhere
                        elide: Text.ElideRight
                        text: (title === "" ? filename : title)
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    Label {
                        text: (length === 0 ? "" : " (" + lengthformatted + ")")
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                    }
                }
                Label {
                    id: artistLabel
                    text: (artist !== "" ? artist + " - " : "") + (album !== "" ? album : "")
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            OpacityRampEffect {
                sourceItem: mainColumn
                slope: 3
                offset: 0.65
            }
            onClicked: {
                albumTracksListView.currentIndex = index
                albumTrackClicked(title, album, artist, lengthformatted, uri,
                                  year, tracknr)
            }
            function playTrackRemorse() {
                remorseAction(qsTr("playing track"), function () {
                    playSong(uri)
                }, 3000)
            }
            function addTrackRemorse() {
                remorseAction(qsTr("adding track"), function () {
                    addSong(uri)
                }, 3000)
            }
            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        text: qsTr("play track")
                        onClicked: {
                            playTrackRemorse()
                        }
                    }

                    MenuItem {
                        text: qsTr("add track to list")
                        onClicked: {
                            addTrackRemorse()
                        }
                    }
                    onHeightChanged: {
                        workaroundHeight = height + mainColumn.height
                    }
                }
            }
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating) {
            lastIndex = albumTracksListView.currentIndex
        } else if (status === PageStatus.Activating) {
            if ( lastIndex > 0 ) {
                albumTracksListView.positionViewAtIndex(lastIndex, ListView.Center)
            }
        } else if (status === PageStatus.Active) {
            if ( lastIndex > 0 ) {
                albumTracksListView.positionViewAtIndex(lastIndex, ListView.Center)
            }
            requestAlbumInfo([albumname, artistname])
            pageStack.pushAttached(Qt.resolvedUrl("AlbumInfoPage.qml"), {
                                       albumname: albumname
                                   })
        }
    }

    Component.onDestruction: {
        albumTracksListView.model = null
        clearTrackList()
    }
}
