import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page
{
    id: savedPlaylistPage
    allowedOrientations: bothOrientation
    property string artistname;
    SilicaListView {
            id : savedPlaylistsListView
            SpeedScroller {
                listview: savedPlaylistsListView
            }
            ScrollDecorator {}
            quickScrollEnabled: jollaQuickscroll
            anchors.fill: parent
//            anchors.bottomMargin: quickControlPanel.visibleSize
            contentWidth: width
            header: PageHeader {
                title: qsTr("playlists")
            }
            model: savedPlaylistsModel
            delegate: ListItem {
                menu: contextMenu
                Column{
                    id: mainColumn
                    anchors {
                        right: parent.right
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                        leftMargin: listPadding
                        rightMargin: listPadding
                    }
                         Label{
                             text: modelData
                        }
                    }
                OpacityRampEffect {
                    sourceItem: mainColumn
                    slope: 3
                    offset: 0.65
                }
                onClicked: {
                    savedPlaylistClicked(modelData);
                    pageStack.push(Qt.resolvedUrl("PlaylistTracksPage.qml"),{playlistname:modelData})
                }
                function playListRemorse() {
                    remorseAction(qsTr("playing list"), function() { playPlaylist(modelData); },3000)
                }
                function addListRemorse() {
                    remorseAction(qsTr("adding list"), function() { addPlaylist(modelData); },3000)
                }
                Component {

                            id: contextMenu
                            ContextMenu {
                                MenuItem {
                                    text: qsTr("play playlist");
                                    onClicked: {
                                        playListRemorse();
                                    }
                                }

                                MenuItem {
                                    text: qsTr("add list")
                                    onClicked: {
                                        addListRemorse();
                                    }
                                }
                            }
                }
            }
    }
    Component.onDestruction: {
        clearPlaylists();
        console.debug("Clearing playlists");
    }
}
