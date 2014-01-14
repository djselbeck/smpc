import QtQuick 2.0
import Sailfish.Silica 1.0
//import harbour.smpc.components 1.0
import "../../components"

Page {
    id: albumslistPage
    allowedOrientations: Orientation.All
    property var listmodel;
    property var tempModel;
    property string artistname
    property int lastIndex;
    SilicaGridView {
        id: albumGridView
        anchors.fill: parent
        clip: true

        SectionScroller {
            listview: albumGridView
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
        delegate: AlbumDelegate{}
    }

    PathView {
        id: showView
        property int itemHeight: parent.height/(1.5)
        property int itemWidth: itemHeight
        anchors.fill: parent
        model: listmodel
        cacheItemCount: pathItemCount*2
        pathItemCount: 12 // width/itemWidth
        delegate: AlbumShowDelegate {}
        snapMode: PathView.NoSnap
        path: Path {
            startX: 0
            startY: showView.height/2
            // Left out
            PathAttribute { name: "z"; value: 0 }
            PathAttribute { name: "delegateRotation"; value: 80 }

            // Left flip (bottom)
            PathLine { x: (showView.width / 2) - (showView.itemWidth /2); y: showView.height-showView.itemHeight/2;  }
            PathAttribute { name: "z"; value: 50 }
            PathAttribute { name: "delegateRotation"; value: 70 }
            PathPercent { value: 0.45 }

            // Center (bottom)
            PathLine { x: (showView.width / 2); y: showView.height-showView.itemHeight/2;  }
            PathAttribute { name: "z"; value: 100 }
            PathAttribute { name: "delegateRotation"; value: 0 }
            PathPercent { value: 0.5 }


            // Right Flip (bottom)
            PathLine { x: (showView.width / 2) + (showView.itemWidth /2); y: showView.height-showView.itemHeight/2;  }
            PathAttribute { name: "z"; value: 50 }
            PathAttribute { name: "delegateRotation"; value: -70 }
            PathPercent { value: 0.55 }

            // Right out
            PathLine { x: showView.width; y: showView.height/2; }
            PathAttribute { name: "z"; value: 0 }
            PathAttribute { name: "delegateRotation"; value: -80 }
            PathPercent { value: 1.0 }

        }
    }

    onStatusChanged: {
        if ( status === PageStatus.Deactivating ) {
            lastIndex = albumGridView.currentIndex;
        }
        else if ( status === PageStatus.Activating ) {
            albumGridView.positionViewAtIndex(lastIndex,ListView.Center);
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


    // New version
    states: [
        State {
            name: "landscape"
            when: orientation === Orientation.Landscape
            PropertyChanges {
                target: albumGridView
                visible: false
            }
            PropertyChanges {
                target: showView
                visible: true
            }
        },
        State {
            name: "portrait"
            when: orientation === Orientation.Portrait
            PropertyChanges {
                target: albumGridView
                visible: false
            }
            PropertyChanges {
                target: showView
                visible: false
            }
        },
        State {
            name: "portraitinverted"
            when: orientation === Orientation.PortraitInverted
            PropertyChanges {
                target: albumGridView
                visible: true
            }
            PropertyChanges {
                target: showView
                visible: false
            }
        },
        State {
            name: "landscapeinverted"
            when: orientation === Orientation.LandscapeInverted
            PropertyChanges {
                target: albumGridView
                visible: false
            }
            PropertyChanges {
                target: showView
                visible: true
            }
        }
    ]

    // UNSTABLE CREATES MEMORY LEAK AS IT CREATES DELEGATES LIKE A MANIAC
//    states: [
//        State {
//            name: "landscape"
//            when: orientation === Orientation.Landscape
//            PropertyChanges {
//                target: albumGridView
//                currentIndex: -1
//                anchors.rightMargin: quickControlPanel.visibleSize
//                anchors.bottomMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//                cellHeight: albumGridView.height
//                cellWidth: cellHeight
//                flow: GridView.TopToBottom
//            }
//        },
//        State {
//            name: "portrait"
//            when: orientation === Orientation.Portrait
//            PropertyChanges {
//                target: albumGridView
//                currentIndex: -1
//                anchors.rightMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//                anchors.bottomMargin: quickControlPanel.visibleSize
//                cellWidth: albumGridView.width / 2
//                cellHeight: cellWidth
//                flow: GridView.LeftToRight
//            }
//        },
//        State {
//            name: "portraitinverted"
//            when: orientation === Orientation.PortraitInverted
//            PropertyChanges {
//                target: albumGridView
//                flow: GridView.LeftToRight
//                cellWidth: albumGridView.width / 2
//                cellHeight: cellWidth
//                //                header: headerComponent
//                anchors.rightMargin: 0
//                anchors.bottomMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: quickControlPanel.visibleSize
//            }
//        },
//        State {
//            name: "landscapeinverted"
//            when: orientation === Orientation.LandscapeInverted
//            PropertyChanges {
//                target: albumGridView
//                flow: GridView.TopToBottom
//                cellHeight: albumGridView.height / 1
//                cellWidth: cellHeight
//                anchors.leftMargin: quickControlPanel.visibleSize
//                anchors.bottomMargin: 0
//                anchors.rightMargin: 0
//                anchors.topMargin: 0
//            }
//        }
//    ]
//    onOrientationChanged: {
//        if ( orientation === Orientation.Landscape ) {
//            // Sets new Gridview parameters
//            // Remove model otherwise GridView goes crazy and creates delegates
//            tempModel = listmodel
//            listmodel = undefined
//            albumListView.cellHeight = albumListView.width
//            albumListView.cellWidth = albumListView.cellHeight
//            albumListView.anchors.leftMargin = 0
//            albumListView.anchors.rightMargin = Qt.binding(function() {return quickControlPanel.visibleSize;})
//            albumListView.anchors.bottomMargin = 0
//            albumListView.anchors.topMargin = 0
//            albumListView.flow = GridView.TopToBottom
//            // Re-set model
//            listmodel = tempModel
//        } else if ( orientation === Orientation.Portrait ) {
//            // Remove model otherwise GridView goes crazy and creates delegates
//            tempModel = listmodel
//            listmodel = undefined
//            albumListView.cellHeight = albumListView.height/2
//            albumListView.cellWidth = albumListView.cellHeight
//            albumListView.anchors.leftMargin = 0
//            albumListView.anchors.rightMargin = 0
//            albumListView.anchors.bottomMargin = Qt.binding(function() {return quickControlPanel.visibleSize;})
//            albumListView.anchors.topMargin = 0
//            albumListView.flow = GridView.LeftToRight
//            // Re-set model
//            listmodel = tempModel
//        }
//    }
}
