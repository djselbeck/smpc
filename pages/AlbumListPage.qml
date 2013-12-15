import QtQuick 2.0
import Sailfish.Silica 1.0
//import harbour.smpc.components 1.0
import "../components"

Page {
    id: albumslistPage
    property var listmodel;
    property string artistname
    property int lastIndex;
    SilicaListView {
        id: albumListView
        quickScrollEnabled: false
        anchors.fill: parent
        highlightFollowsCurrentItem: false
        model: visible ? listmodel : null
        SectionScroller {
            listview: albumListView
        }

        header: PageHeader {
            title: artistname === "" ? qsTr("albums") : artistname
        }
        PullDownMenu {
            enabled: artistname !== ""
            MenuItem {
                text: qsTr("add albums")
                onClicked: {
                    addArtist(artistname)
                }
            }
            MenuItem {
                text: qsTr("play albums")
                onClicked: {
                    playArtist(artistname)
                }
            }
        }
        delegate: AlbumDelegate {
        }

        section {
            property: 'sectionprop'
            delegate: Component {
                SectionHeader {
                    text: section
                }
            }
        }
    }

    onStatusChanged: {
        if ( status === PageStatus.Deactivating ) {
            lastIndex = albumListView.currentIndex;
        }
        else if ( status === PageStatus.Activating ) {
            albumListView.positionViewAtIndex(lastIndex,ListView.Center);
        }
    }

    Component.onDestruction: {
        clearAlbumList();
    }
}
