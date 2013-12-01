import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: searchpage
    property alias listmodel: albumsongs_list_view.model
    property int currentindex: -1
    property string selectedsearch



    SilicaFlickable {
        id: mainSearchFlickable
        contentHeight: albumsongs_list_view.contentHeight + searchhead.height
        anchors.fill: parent
        ScrollDecorator {
        }
        Column {
            id: searchhead
            spacing: Theme.paddingMedium
            clip: true
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            PageHeader {
                title: qsTr("search")
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            TextField {
                id: searchfield
                anchors {
                    left: parent.left
                    right: parent.right
                }
                placeholderText: qsTr("search value")
                text: ""
                inputMethodHints: Qt.ImhNoPredictiveText
            }
            Button {
                id: startsearchbtn
                text: qsTr("Search")
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: listPadding
                    rightMargin: listPadding
                }
                onClicked: {
                    var searchfor
                    switch (searchforcombobox.currentIndex) {
                    case 0:
                        console.debug("Searching for titles")
                        searchfor = "title"
                        break
                    case 1:
                        console.debug("Searching for albums")
                        searchfor = "album"
                        break
                    case 2:
                        console.debug("Searching for artists")
                        searchfor = "artist"
                        break
                    case 3:
                        console.debug("Searching for files")
                        searchfor = "file"
                        break
                    }

                    requestSearch([searchfor, searchfield.text])
                    albumsongs_list_view.forceActiveFocus()
                }
            }
            ComboBox {
                id: searchforcombobox
                label: qsTr("Search for:")
                anchors {
                    left: parent.left
                    right: parent.right
                }
                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("titles")
                    }
                    MenuItem {
                        text: qsTr("albums")
                    }
                    MenuItem {
                        text: qsTr("artists")
                    }
                    MenuItem {
                        text: qsTr("files")
                    }
                }
            }
        }

        SilicaListView {
            id: albumsongs_list_view
            contentHeight: 0
            height: contentHeight
            interactive: false

            delegate: ListItem {
                menu: contextMenu
                Column {
                    clip: true
                    anchors {
                        right: parent.right
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                        leftMargin: listPadding
                        rightMargin: listPadding
                    }
                    Row {
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
                            text: (length === 0 ? "" : " (" + lengthformated + ")")
                            anchors {
                                verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                    Label {
                        text: (artist !== "" ? artist + " - " : "") + (album !== "" ? album : "")
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }
                onClicked: {
                    albumTrackClicked(title, album, artist, lengthformated,
                                      uri, year, tracknr)
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

                function playAlbumRemorse() {
                    remorseAction(qsTr("playing album"), function () {
                        playAlbum(["", album])
                    }, 3000)
                }
                function addAlbumRemorse() {
                    remorseAction(qsTr("adding album"), function () {
                        addAlbum(["", album])
                    }, 3000)
                }
                Component {
                    id: contextMenu
                    ContextMenu {
                        property int lastHeight: 0
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
                        MenuItem {
                            text: qsTr("add album to list")
                            onClicked: {
                                addAlbumRemorse()
                            }
                        }
                        MenuItem {
                            text: qsTr("play album")
                            onClicked: {
                                playAlbumRemorse()
                            }
                        }


                        onYChanged: {
                            if ( parent != null) {
                                var menuPos = parent.y+albumsongs_list_view.currentItem.y+albumsongs_list_view.y-mainSearchFlickable.contentY;
                                var menuMax = menuPos + parent.height;
                                if(menuMax > mainSearchFlickable.height) {
                                    mainSearchFlickable.contentY += menuMax-mainSearchFlickable.height;
                                    console.debug("cy: " + mainSearchFlickable.contentY );
                                }

                            }
                        }

                    }
                }
            }
            anchors {
                left: parent.left
                right: parent.right
                top: searchhead.bottom
            } //; bottom: parent.bottom }
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("add all results")
                visible: albumsongs_list_view.model !== undefined
                onClicked: {
                    deletePlaylist()
                    addlastsearch()
                }
            }
            MenuItem {
                text: qsTr("play all results")
                visible: albumsongs_list_view.model !== undefined
                onClicked: {
                    deletePlaylist()
                    addlastsearch()
                    playPlaylistTrack(0)
                }
            }
        }
    }
}
