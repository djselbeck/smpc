import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: artistlistPage
    property var listmodel;
    property int lastIndex;

    SilicaGridView {
        model: listmodel
            id : artistListView
            cellWidth: width/2
            cellHeight: width/2

            SectionScroller{
                listview: artistListView
                sectionPropertyName: "sectionprop"
            }
            ScrollDecorator {}
            anchors.fill: parent
            header: PageHeader {
                title: qsTr("artists");
            }
            delegate: ArtistDelegate{}

//            section {
//                property: 'sectionprop'
//                delegate: SectionHeader {
//                    text: section
//                }
//            }
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
