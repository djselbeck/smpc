import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: albumTracksPage
    property alias listmodel: albumTracksListView.model;
    property string albumname;
    property string artistname;
    property int lastIndex;
    SilicaListView {
            id : albumTracksListView
            ScrollDecorator {}
            anchors.fill: parent
            contentWidth: width
            header: PageHeader {
                title: albumname;
            }
            PullDownMenu {
                MenuItem {
                    text: qsTr("add album")
                    onClicked: {
                        addAlbum([artistname,albumname]);
                    }
             }
                MenuItem {
                    text: qsTr("play album")
                    onClicked: {
                        playAlbum([artistname,albumname]);
                    }
             }
            }
            delegate: ListItem {
                menu: contextMenu
                Column{
                    id: mainColumn
                    clip: true
                    anchors {
                        right: parent.right
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                        leftMargin: listPadding
                        rightMargin: listPadding
                    }
                        Row{
                            Label {text: (index+1)+". ";anchors {verticalCenter: parent.verticalCenter}}
                            Label {clip: true; wrapMode: Text.WrapAnywhere; elide: Text.ElideRight; text:  (title==="" ? filename : title);anchors {verticalCenter: parent.verticalCenter}}
                            Label { text: (length===0 ? "": " ("+lengthformated+")");anchors {verticalCenter: parent.verticalCenter}}
                        }
                        Label{
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
                    albumTracksListView.currentIndex = index;
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
                    }
                }
            }
    }

    onStatusChanged: {
        if ( status === PageStatus.Deactivating ) {
            lastIndex = albumTracksListView.currentIndex;
        }
        else if ( status === PageStatus.Activating || status === PageStatus.Active ) {
            albumTracksListView.positionViewAtIndex(lastIndex,ListView.Center);
        }
    }

    Component.onDestruction: {
        albumTracksListView.model = null;
        clearTrackList();
    }

}
