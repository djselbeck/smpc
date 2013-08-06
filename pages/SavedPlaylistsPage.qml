import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: savedPlaylistPage
    property alias listmodel: savedPlaylistsListView.model;
    property string artistname;
    SilicaListView {
            id : savedPlaylistsListView
            anchors.fill: parent
            contentWidth: width
            header: PageHeader {
                title: qsTr("playlists")
            }
            delegate: BackgroundItem {
                Column{
                    x : Theme.paddingLarge
                    anchors.verticalCenter: parent.verticalCenter
                         Label{
                             text: modelData
                        }
                    }
                onClicked: {
                    savedPlaylistClicked(modelData);
                }
            }
    }

}
