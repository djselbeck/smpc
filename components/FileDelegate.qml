import QtQuick 2.0
import Sailfish.Silica 1.0

Component {
    ListItem {
        menu: contextMenu
        id: filesDelegate
        Image {
            anchors {
                leftMargin: listPadding
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            id: fileicon
            source: (isDirectory ? "image://theme/icon-l-storage" : "image://theme/icon-m-music")
            width: height
        }
        Column {
            id: mainColumn
            clip: true
            anchors {
                left: fileicon.right
                right: parent.right
                verticalCenter: parent.verticalCenter
                rightMargin: listPadding
            }
            Label {
                id: filenametext
                text: name
                wrapMode: "NoWrap"
                anchors {
                    left: parent.left
                    right: parent.right
                }
                elide: Text.ElideMiddle
            }
            Label {
                visible: isFile
                text: (!isFile ? "" : (title === "" ? "" : title + " - ")
                                 + (artist === "" ? "" : artist))
                anchors {
                    left: parent.left
                    right: parent.right
                }
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
            }
        }
        OpacityRampEffect {
            sourceItem: mainColumn
            slope: 3
            offset: 0.65
        }
        onClicked: {
            if (isDirectory) {
                lastContentPosY = index
                filesClicked((prepath == "/" ? "" : prepath + "/") + name)
            }
            if (isFile) {
                console.debug("Album:" + album)
                albumTrackClicked(title, album, artist, length, path,
                                  year, tracknr)
            }
            if (isPlaylist) {
                console.debug(
                            "Playlist:" + (prepath == "/" ? "" : prepath + "/") + name)
                savedPlaylistClicked(
                            (prepath == "/" ? "" : prepath + "/") + name)
            }
        }

        function addTrackRemorse() {
            remorseAction(qsTr("adding track"), function () {
                addSong(path)
            }, 3000)
        }
        function addPlaylistRemorse() {
            remorseAction(qsTr("adding playlist"), function () {
                console.debug(
                            "playlist: " + (prepath == "/" ? "" : prepath + "/")
                            + name + " requested")
                addPlaylist((prepath == "/" ? "" : prepath + "/") + name)
            }, 3000)
        }
        function addFolderRemorse() {
            remorseAction(qsTr("adding directory"), function () {
                addFiles((prepath == "/" ? "" : prepath + "/") + name)
            }, 3000)
        }
        function playTrackRemorse() {
            remorseAction(qsTr("playing track"), function () {
                playSong(path)
            }, 3000)
        }
        function playPlaylistRemorse() {
            remorseAction(qsTr("playing playlist"), function () {
                playPlaylist((prepath == "/" ? "" : prepath + "/") + name)
            }, 3000)
        }
        function playFolderRemorse() {
            remorseAction(qsTr("playing directory"), function () {
                playFiles((prepath == "/" ? "" : prepath + "/") + name)
            }, 3000)
        }
        Component {
            id: contextMenu
            ContextMenu {
                MenuItem {
                    text: isFile ? qsTr(
                                       "play file") : (isDirectory ? qsTr("play directory") : qsTr(
                                                                         "play playlist"))
                    onClicked: {
                        if (isFile) {
                            playTrackRemorse()
                        } else if (isDirectory) {
                            playFolderRemorse()
                        } else if (isPlaylist) {
                            playPlaylistRemorse()
                        }
                    }
                }

                MenuItem {
                    text: isFile ? qsTr(
                                       "add file") : (isDirectory ? qsTr("add directory") : qsTr(
                                                                        "add playlist"))
                    onClicked: {
                        if (isFile) {
                            addTrackRemorse()
                        } else if (isDirectory) {
                            addFolderRemorse()
                        } else if (isPlaylist) {
                            addPlaylistRemorse()
                        }
                    }
                }
            }
        }
    }
}
