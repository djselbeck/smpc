import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page {
    id: artistlistPage
    property var listmodel
    property int lastIndex

    SilicaGridView {
        id: artistListView
        cellWidth: width / 2
        cellHeight: width / 2
        anchors.fill: parent
        model: listmodel

        SectionScroller {
            listview: artistListView
            sectionPropertyName: "sectionprop"
        }
        ScrollDecorator {
        }

        header: PageHeader {
            title: qsTr("artists")
        }
        delegate: ArtistDelegate {
        }

    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating) {
            lastIndex = artistListView.currentIndex
        } else if (status === PageStatus.Activating) {
            artistListView.positionViewAtIndex(lastIndex, ListView.Center)
        }
    }

    Component.onDestruction: {
        artistListView.model = null
        clearArtistList()
    }
}
