import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: playlistTracksPage
    property var listmodel;
    property string playlistname;
    property int lastIndex;
    SilicaListView {
            id : playlistTracksListView
            model: listmodel
            SpeedScroller {
                listview: playlistTracksListView
            }
            anchors.fill: parent
            contentWidth: width
            header: PageHeader {
                title: playlistname;
            }
            PullDownMenu {
                MenuItem {
                    text: qsTr("delete list")
                    onClicked: {
                        deleteSavedPlaylistQuestion.playlistname = playlistname;
                        pageStack.openDialog(deleteSavedPlaylistQuestion);
                    }
             }
                MenuItem {
                    text: qsTr("add list")
                    onClicked: {
                        addPlaylist(playlistname)
                    }
                }
                MenuItem {
                    text: qsTr("play list")
                    onClicked: {
                        playPlaylist(playlistname)
                    }
                }
            }
            delegate: ListItem {
                menu: contextMenu
                property int workaroundHeight: mainColumn.height
                height: workaroundHeight
                Column{
                    id: mainColumn
                    height: (trackRow + artistLabel >= Theme.itemSizeSmall ?trackRow + artistLabel : Theme.itemSizeSmall )
                    clip: true
                    anchors {
                        right: parent.right
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                        leftMargin: listPadding
                        rightMargin: listPadding
                    }
                        Row{
                            id: trackRow
                            Label {text: (index+1)+". ";anchors {verticalCenter: parent.verticalCenter}}
                            Label {clip: true; wrapMode: Text.WrapAnywhere; elide: Text.ElideRight; text:  (title==="" ? filename : title);anchors {verticalCenter: parent.verticalCenter}}
                            Label { text: (length===0 ? "": " ("+lengthformated+")");anchors {verticalCenter: parent.verticalCenter}}
                        }
                        Label{
                            id: artistLabel
                            text:(artist!=="" ? artist + " - " : "" )+(album!=="" ? album : "");
                            color: Theme.secondaryColor;
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                OpacityRampEffect {
                    sourceItem: mainColumn
                    slope: 3
                    offset: 0.65
                }
                onClicked: {
                    playlistTracksListView.currentIndex = index;
                    albumTrackClicked(title,album,artist,lengthformated,uri,year,tracknr);
                }
                function playTrackRemorse() {
                    remorseAction(qsTr("playing track"), function() { playSong(uri); },3000)
                }
                function addTrackRemorse() {
                    remorseAction(qsTr("adding track"), function() { addSong(uri); },3000)
                }
                Component {
                    id: contextMenu
                    ContextMenu {
                        MenuItem {
                            text: qsTr("play track");
                            onClicked: {
                                playTrackRemorse();
                            }
                        }

                        MenuItem {
                            text: qsTr("add track to list")
                            onClicked: {
                                addTrackRemorse();
                            }
                        }
                        onHeightChanged: {
                            workaroundHeight = height + mainColumn.height
                        }
                    }
                }
            }
    }
    Dialog
    {
        id: deleteSavedPlaylistQuestion
        property string playlistname;
        Column {
            width: parent.width
            spacing: 10
            anchors.margins: Theme.paddingMedium
            DialogHeader {
                acceptText: qsTr("delete playlist");
            }
            Label {
                text: qsTr("really delete playlist?")
             }
        }
        onDone: {
            if ( result == DialogResult.Accepted)
            {
                deleteSavedPlaylist(playlistname);
                pageStack.clear();
                pageStack.push(initialPage);
            }
        }
    }

    onStatusChanged: {
        if ( status === PageStatus.Deactivating ) {
            lastIndex = playlistTracksListView.currentIndex;
        }
        else if ( status === PageStatus.Activating ) {
            playlistTracksListView.positionViewAtIndex(lastIndex,ListView.Center);
        }
    }

    Component.onDestruction: {
        playlistTracksListView.model = null;
        clearPlaylistTracks();
    }

}
