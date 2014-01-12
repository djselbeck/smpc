import QtQuick 2.0
import Sailfish.Silica 1.0
//import harbour.smpc.components 1.0
import "../../components"

Page {
    id: albumslistPage
    allowedOrientations: Orientation.All
    property var listmodel;
    property string artistname
    property int lastIndex;
    SilicaGridView {
        id: albumListView
        anchors.fill: parent
        model: listmodel
        clip: true

        SectionScroller {
            listview: albumListView
            sectionPropertyName: "sectionprop"
        }
        ScrollDecorator {}

        header: Heading {
            text: qsTr("albums")
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
    }

    onStatusChanged: {
        if ( status === PageStatus.Deactivating ) {
            lastIndex = albumListView.currentIndex;
        }
        else if ( status === PageStatus.Activating ) {
            albumListView.positionViewAtIndex(lastIndex,ListView.Center);
            if(artistname != "") {
                requestArtistInfo(artistname);
            }
        }
        else if ( status === PageStatus.Active ) {
            if(artistname!="")
                pageStack.pushAttached(Qt.resolvedUrl("ArtistInfoPage.qml"),{artistname:artistname});
        }
    }

    Component.onDestruction: {
        clearAlbumList();
    }
    states: [
        State {
            name: "landscape"
            PropertyChanges {
                target: albumListView
                flow: GridView.TopToBottom
                cellHeight: albumListView.height / 1
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
                target: albumListView
                flow: GridView.LeftToRight
                cellWidth: albumListView.width / 2
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
                target: albumListView
                flow: GridView.LeftToRight
                cellWidth: albumListView.width / 2
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
                target: albumListView
                flow: GridView.TopToBottom
                cellHeight: albumListView.height / 1
                cellWidth: cellHeight
                anchors.leftMargin: quickControlPanel.visibleSize
                anchors.bottomMargin: 0
                anchors.rightMargin: 0
                anchors.topMargin: 0
            }
        }
    ]
    onOrientationChanged: {
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
