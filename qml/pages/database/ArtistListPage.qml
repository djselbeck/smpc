import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page {
    id: artistlistPage
    property var listmodel
    property int lastIndex
    allowedOrientations: Orientation.All

    SilicaGridView {
        id: artistListView
        anchors.fill: parent
        model: listmodel
        clip: true

        SectionScroller {
            listview: artistListView
            sectionPropertyName: "sectionprop"
        }
        ScrollDecorator {}


        header: Heading {
            text: qsTr("artists")
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

    states: [
        State {
            name: "landscape"
            PropertyChanges {
                target: artistListView
                flow: GridView.TopToBottom
                cellHeight: artistListView.height / 1
                cellWidth: cellHeight
                anchors.rightMargin: quickControlPanel.visibleSize
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
            }
        },
        State {
            name: "portrait"
            PropertyChanges {
                target: artistListView
                flow: GridView.LeftToRight
                cellWidth: artistListView.width / 2
                cellHeight: cellWidth
                //                header: headerComponent
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                anchors.bottomMargin: quickControlPanel.visibleSize
            }
        },
        State {
            name: "portraitinverted"
            PropertyChanges {
                target: artistListView
                flow: GridView.LeftToRight
                cellWidth: artistListView.width / 2
                cellHeight: cellWidth
                //                header: headerComponent
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: quickControlPanel.visibleSize
            }
        },
        State {
            name: "landscapeinverted"
            PropertyChanges {
                target: artistListView
                flow: GridView.TopToBottom
                cellHeight: artistListView.height / 1
                cellWidth: cellHeight
                anchors.leftMargin: quickControlPanel.visibleSize
                anchors.bottomMargin: 0
                anchors.rightMargin: 0
                anchors.topMargin: 0
            }
        }
    ]
    onOrientationChanged: {
        // Scroll to first element, otherwise GridView starts creating
        // delegates like a maniac
        artistListView.currentIndex = -1
        artistListView.positionViewAtIndex(0,GridView.Beginning)
        switch ( orientation ) {
        case Orientation.Portrait :
            state = "portrait"
            break
        case Orientation.Landscape :
            state = "landscape"
            break
        case Orientation.PortraitInverted :
            state = "portraitinverted"
            break
        case Orientation.LandscapeInverted:
            state = "landscapeinverted"
            break
        }
    }
    Component.onCompleted: {
        switch ( orientation ) {
        case Orientation.Portrait :
            state = "portrait"
            break
        case Orientation.Landscape :
            state = "landscape"
            break
        case Orientation.PortraitInverted :
            state = "portraitinverted"
            break
        case Orientation.LandscapeInverted:
            state = "landscapeinverted"
            break
        }
    }


}
