import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page {
    id: artistlistPage
    property int lastIndex
    property int lastOrientation
    allowedOrientations: bothOrientation

    Loader {
        id: gridViewLoader
        anchors.fill: parent
//        anchors.bottomMargin: quickControlPanel.visibleSize
        active: false

        sourceComponent: Component {
            SilicaGridView {
                id: artistListView

                model: artistsModel
                clip: true
                cellWidth: width/2
                cellHeight: cellWidth
                populate: Transition {
                    NumberAnimation { properties: "x"; from:artistListView.width*2 ;duration: populateDuration }
                }

                SectionScroller {
                    listview: artistListView
                    sectionPropertyName: "sectionprop"
                }
                ScrollDecorator {
                }

                header: Heading {
                    text: qsTr("artists")
                    width: parent.width
                    height: Theme.itemSizeMedium
                }

                delegate: ArtistDelegate {
                }
            }
        }
    }

    Loader {
        id: showViewLoader
        active: false
        anchors.fill: parent
//        anchors.rightMargin: quickControlPanel.visibleSize
        sourceComponent: Component {
            PathView {
                id: showView
                property int itemHeight: height / (1.3)
                property int itemWidth: itemHeight
                onHeightChanged: {
                    console.debug("height: "+height)
                }
                onWidthChanged: {
                    console.debug("width: " + width)
                }
                model: artistsModel

                SectionScroller {
                    pathview: showView
                    sectionPropertyName: "sectionprop"
                    z: 120
                    interactive: showView.interactive
                }



                cacheItemCount: pathItemCount + 2
                pathItemCount: 12 // width/itemWidth
                delegate: ArtistShowDelegate {
                }
                snapMode: PathView.NoSnap
                preferredHighlightBegin: 0.5
                preferredHighlightEnd: 0.5
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
            if (!orientationTransitionRunning && orientation != lastOrientation) {
                gridViewLoader.active = false;
                showViewLoader.active = false;
                if (orientation === Orientation.Portrait) {
                    console.debug("activating page with portrait grid view")
                    gridViewLoader.active = true
                } else if (orientation === Orientation.Landscape) {
                    console.debug("activating page landscape showview")
                    showViewLoader.active = true
                }
            }
        }
        if (status === PageStatus.Deactivating ) {
            if (typeof(gridViewLoader.item) != undefined && gridViewLoader.item) {
                lastIndex = gridViewLoader.item.currentIndex
            }
            lastOrientation  = orientation
        } else if (status === PageStatus.Activating && typeof(gridViewLoader.item) != undefined && gridViewLoader.item) {
            gridViewLoader.item.positionViewAtIndex(lastIndex, GridView.Center)
        }
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
        //artistListView.model = null
        clearArtistList()
    }

}
