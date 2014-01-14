import QtQuick 2.0
import Sailfish.Silica 1.0
//import harbour.smpc.components 1.0
import "../../components"

Page {
    id: albumslistPage
    allowedOrientations: Orientation.All
    property var listmodel
    property var tempModel
    property string artistname
    property int lastIndex
    Loader {
        id: gridViewLoader
        active: false
        anchors.fill: albumslistPage
        anchors.bottomMargin: quickControlPanel.visibleSize

        sourceComponent: Component {
            SilicaGridView {
                id: albumGridView
                clip: true
                model: listmodel
                cellWidth: width / 2
                cellHeight: cellWidth
                SectionScroller {
                    listview: albumGridView
                    sectionPropertyName: "sectionprop"
                }
                ScrollDecorator {
                }

                //                header: Heading {
                //                    text: qsTr("albums")
                //                    width: parent.width
                //                    height: Theme.itemSizeMedium
                //                }
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
        }
    }

    Loader {
        id: showViewLoader
        active: false
        anchors.fill: parent
        anchors.rightMargin: quickControlPanel.visibleSize
        sourceComponent: Component {
            PathView {
                id: showView
                property int itemHeight: height / (1.5)
                property int itemWidth: itemHeight
                onHeightChanged: {
                    console.debug("height: "+height)
                }
                onWidthChanged: {
                    console.debug("width: " + width)
                }

                model: listmodel
                cacheItemCount: pathItemCount * 2
                pathItemCount: 12 // width/itemWidth
                delegate: AlbumShowDelegate {
                }
                snapMode: PathView.NoSnap
                clip: true
                path: Path {
                    startX: 0
                    startY: showView.height / 2
                    // Left out
                    PathAttribute {
                        name: "z"
                        value: 0
                    }
                    PathAttribute {
                        name: "delegateRotation"
                        value: 80
                    }

                    // Left flip (bottom)
                    PathLine {
                        x: (showView.width / 2) - (showView.itemWidth / 2)
                        y: showView.height - showView.itemHeight / 2
                    }
                    PathAttribute {
                        name: "z"
                        value: 50
                    }
                    PathAttribute {
                        name: "delegateRotation"
                        value: 70
                    }
                    PathPercent {
                        value: 0.45
                    }

                    // Center (bottom)
                    PathLine {
                        x: (showView.width / 2)
                        y: showView.height - showView.itemHeight / 2
                    }
                    PathAttribute {
                        name: "z"
                        value: 100
                    }
                    PathAttribute {
                        name: "delegateRotation"
                        value: 0
                    }
                    PathPercent {
                        value: 0.5
                    }

                    // Right Flip (bottom)
                    PathLine {
                        x: (showView.width / 2) + (showView.itemWidth / 2)
                        y: showView.height - showView.itemHeight / 2
                    }
                    PathAttribute {
                        name: "z"
                        value: 50
                    }
                    PathAttribute {
                        name: "delegateRotation"
                        value: -70
                    }
                    PathPercent {
                        value: 0.55
                    }

                    // Right out
                    PathLine {
                        x: showView.width
                        y: showView.height / 2
                    }
                    PathAttribute {
                        name: "z"
                        value: 0
                    }
                    PathAttribute {
                        name: "delegateRotation"
                        value: -80
                    }
                    PathPercent {
                        value: 1.0
                    }
                }
            }
        }
    }

    onStatusChanged: {
        if ( status === PageStatus.Activating ) {
            if (!orientationTransitionRunning) {
                if (orientation === Orientation.Portrait) {
                    console.debug("activating portrait grid view")
                    gridViewLoader.active = true
                } else if (orientation === Orientation.Landscape) {
                    console.debug("activating landscape showview")
                    showViewLoader.active = true
                }
            } else {
                console.debug("deactivating loaders")
                gridViewLoader.active = false
                showViewLoader.active = false
                // Deactivating components
            }
        }

        //        if (status === PageStatus.Deactivating) {
        //            lastIndex = albumGridView.currentIndex
        //        } else if (status === PageStatus.Activating) {
        //            albumGridView.positionViewAtIndex(lastIndex, ListView.Center)
        //            if (artistname != "") {
        //                requestArtistInfo(artistname)
        //            }
        //        } else if (status === PageStatus.Active) {
        //            if (artistname != "")
        //                pageStack.pushAttached(Qt.resolvedUrl("ArtistInfoPage.qml"), {
        //                                           artistname: artistname
        //                                       })
        //        }
    }



    onOrientationTransitionRunningChanged: {
        if (!orientationTransitionRunning) {
            if (orientation === Orientation.Portrait) {
                console.debug("activating portrait grid view")
                gridViewLoader.active = true
            } else if (orientation === Orientation.Landscape) {
                console.debug("activating landscape showview")
                showViewLoader.active = true
            }
        } else {
            console.debug("deactivating loaders")
            gridViewLoader.active = false
            showViewLoader.active = false
            // Deactivating components
        }
    }

    Component.onDestruction: {
        clearAlbumList()
    }

}
