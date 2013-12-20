import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: currentPlaylistPage
    //property alias listmodel: playlistView.model
    property alias songid: playlistView.currentIndex
    property int lastIndex
    SilicaListView {
        id: playlistView
        delegate: trackDelegate
        anchors.fill: parent
        model: playlistModelVar

        highlightFollowsCurrentItem: true
        highlightMoveDuration: 1200
        header: PageHeader {
            title: qsTr("playlist")
        }
        PullDownMenu {
            MenuItem {
                text: qsTr("delete playlist")
                onClicked: {
                    pageStack.openDialog(deleteQuestionDialog)
                }
            }
            MenuItem {
                text: qsTr("save playlist")
                onClicked: {
                    pageStack.openDialog(saveplaylistDialog)
                }
            }
            MenuItem {
                text: qsTr("open playlist")
                onClicked: {
                    requestSavedPlaylists()
                }
            }
            MenuItem {
                text: qsTr("jump to playing song")
                onClicked: {
                    playlistView.currentIndex = -1
                    playlistView.currentIndex = lastsongid
                }
            }
        }

        SpeedScroller {
            listview: playlistView
        }
        ScrollDecorator {}
        Component {
            id: trackDelegate
            ListItem {
                property int workaroundHeight: mainColumn.height
                height: workaroundHeight
                menu: contextMenu
                Component {
                    id: contextMenu
                    ContextMenu {
                        MenuItem {
                            visible: !playing
                            text: qsTr("play song")
                            onClicked: playPlaylistTrack(index)
                        }
                        MenuItem {
                            text: qsTr("remove song")
                            onClicked: {
                                remove()
                            }
                        }

                        MenuItem {
                            visible: playing
                            text: qsTr("show information")
                            onClicked: pageStack.push(currentsongpage)
                        }

                        onHeightChanged: {
                            workaroundHeight = height + mainColumn.height
                        }
                    }
                }
                //                Component.onCompleted: {
                //                    console.debug("component created: " + title);
                //                }
                Column {
                    id: mainColumn
                    clip: true
                    height: (trackRow + artistLabel >= Theme.itemSizeSmall ?trackRow + artistLabel : Theme.itemSizeSmall )
                    anchors {
                        right: parent.right
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                        leftMargin: listPadding
                        rightMargin: listPadding
                    }
                    Row {
                        id: trackRow
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
                            text: (title === "" ? filename + " " : title + " ")
                            font.italic: (playing) ? true : false
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
                GlassItem {
                    anchors.fill: parent
                    visible: playing
                    scale: 0.5
                }
                onClicked: {
                    playlistView.currentIndex = index
                    if (!playing) {
                        parseClickedPlaylist(index)
                    } else {
                        pageStack.push(currentsongpage)
                    }
                }

                function remove() {
                    remorseAction(qsTr("Deleting"), function () {
                        deletePlaylistTrack(index)
                    }, 3000)
                }
            }
        }
    }

    // Delete question
    Dialog {
        id: deleteQuestionDialog
        Column {
            width: parent.width
            spacing: 10
            anchors.margins: Theme.paddingMedium
            DialogHeader {
                acceptText: qsTr("delete playlist")
            }
            Label {
                text: qsTr("really delete playlist?")
            }
        }
        onDone: {
            if (result === DialogResult.Accepted) {
                deletePlaylist()
            }
        }
    }

    Dialog {
        id: saveplaylistDialog
        Column {
            width: parent.width
            spacing: 10
            anchors.margins: Theme.paddingMedium
            DialogHeader {
                acceptText: qsTr("save playlist")
            }
            Label {
                text: qsTr("playlist name:")
            }
            TextField {
                id: playlistNameField
                width: parent.width
                placeholderText: qsTr("input playlist name")
            }
        }
        onDone: {
            if (result === DialogResult.Accepted) {
                console.debug("Saving playlist: " + playlistNameField.text)
                savePlaylist(playlistNameField.text)
            }
            playlistNameField.text = ""
            playlistNameField.focus = false
        }
        onOpened: {
            playlistNameField.focus = true
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Activating) {
            playlistView.positionViewAtIndex(lastsongid, ListView.Center)
        }
        else if ( status === PageStatus.Active ) {
            pageStack.pushAttached(currentsongpage);
        }
    }

    function parseClickedPlaylist(index) {
        playPlaylistTrack(index)
    }
}
