import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: artistlistPage
    property var listmodel;
    property int lastIndex;
    SilicaListView {
        model: listmodel
            id : artistListView
            SectionScroller{
                listview: artistListView
            }
            anchors.fill: parent
            header: PageHeader {
                title: qsTr("artists");
            }
            delegate: ArtistDelegate{}
            section {
                property: 'sectionprop'
                delegate: SectionHeader {
                    text: section
                }
            }
    }

    onStatusChanged: {
        if ( status === PageStatus.Deactivating ) {
            lastIndex = artistListView.currentIndex;
        }
        else if ( status === PageStatus.Activating ) {
            artistListView.positionViewAtIndex(lastIndex,ListView.Center);
        }
    }

    Component.onDestruction: {
        artistListView.model = null;
        clearArtistList();
    }



}
