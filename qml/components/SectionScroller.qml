import QtQuick 2.0
import Sailfish.Silica 1.0
import "SectionScroller.js" as Sections

Item {
    id: scroller
    property ListView listview;
    property GridView gridView;
    property PathView pathview;
    property bool interactive:true
    property int cacheCount: 0
    property string sectionPropertyName
    property bool landscape:false


    Rectangle {
        id: testrect
        visible: mDebugEnabled
        opacity:0.5
        anchors.fill:parent
    }

    Rectangle {
        id: sectionScrollIndicator
        property bool listviewScrolling : ( listview && listview.flicking)
        property bool gridviewScrolling : ( gridView && gridView.flicking)
        property bool pathviewScrolling : ( pathview && pathview.flicking)
        property bool inputAreaScrolling : inputArea.pressed
        opacity: ( (listviewScrolling || gridviewScrolling || pathviewScrolling || inputAreaScrolling) && !landscape ? 1.0 : 0.0 )
        anchors.fill:parent

        gradient: Gradient {

            GradientStop {
                position: 0.0
                color: Qt.rgba(0.0, 0.0, 0.0, 0.0)
            }
            GradientStop {
                position: 0.5
                color: Theme.rgba(Theme.highlightBackgroundColor,0.4)
            }
            GradientStop {
                position: 1.0
                color: Qt.rgba(0.0, 0.0, 0.0, 0.0)
            }
        }
        Behavior on opacity {
                 PropertyAnimation { properties: "opacity"; easing.type: Easing.InOutQuad; duration: 300 }
        }
    }

    Rectangle {
        id: sectionScrollIndicatorLandscape
        property bool listviewScrolling : ( listview && listview.flicking)
        property bool gridviewScrolling : ( gridView && gridView.flicking)
        property bool pathviewScrolling : ( pathview && pathview.flicking)
        property bool inputAreaScrolling : inputArea.pressed
        opacity: ( (listviewScrolling || gridviewScrolling || pathviewScrolling || inputAreaScrolling) && landscape ? 1.0 : 0.0 )
        anchors.fill:parent
        color: Theme.rgba(Theme.highlightBackgroundColor,0.4)
        Behavior on opacity {
                 PropertyAnimation { properties: "opacity"; easing.type: Easing.InOutQuad; duration: 300 }
        }
    }

    onListviewChanged: {
        if(listview && listview.model) {
            cacheCount = listview.cacheBuffer
            Sections.fillSections(listview,scroller.sectionPropertyName);
        } else if(listview) {
            cacheCount = listview.cacheBuffer
            listview.modelChanged.connect( function() {
                Sections.fillSections(listview,scroller.sectionPropertyName);
            });

        }
    }

    onPathviewChanged: {
        if(pathview && pathview.model) {
            Sections.fillSections(pathview,scroller.sectionPropertyName);
        } else if(pathview) {
            pathview.modelChanged.connect( function() {
                Sections.fillSections(pathview,scroller.sectionPropertyName);
            });

        }
    }

    onGridViewChanged: {
        if(gridView && gridView.model) {
            cacheCount = gridView.cacheBuffer
            Sections.fillSections(gridView,scroller.sectionPropertyName);
        } else if(gridView) {
            cacheCount = gridView.cacheBuffer
            gridView.modelChanged.connect( function() {
                Sections.fillSections(gridView,scroller.sectionPropertyName);
            });

        }
    }

    Rectangle {
        id: secDialog
        property alias text: currSecText.text
        y: landscape ? -height : (inputArea.mouseY - (height / 2)
                                  > (parent.height - height) ? (parent.height - height) : (inputArea.mouseY - (height / 2) < 0 ? 0 : inputArea.mouseY - (height / 2)))
        x: landscape ? (inputArea.mouseX - (width / 2) > (parent.width - width) ? (parent.width - width) : (inputArea.mouseX - (width / 2) < 0 ? 0 : inputArea.mouseX - (width / 2))) : -width
        height: currSecText.height
        width: currSecText.width+(Theme.paddingMedium*2)
        visible:true
        opacity:0.0
        Behavior on opacity {
                 PropertyAnimation { properties: "opacity"; easing.type: Easing.InOutQuad; duration: 500 }
        }
        //color: "#77222222"
        color: Theme.highlightBackgroundColor
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            id: currSecText
            color: Theme.primaryColor
            opacity: 1.0
            font.pointSize: Theme.fontSizeHuge
            text: ""
        }
    }

    MouseArea {
        id: inputArea
        anchors.fill:parent
        preventStealing: interactive
        enabled: interactive
        onPressedChanged: {
            if ( interactive && pressed ) {
                secDialog.color = Theme.rgba(Theme.highlightBackgroundColor,0.5);
                secDialog.opacity = 1.0;
                //secDialog.visible = true;
                if ( listview && typeof( listview ) != undefined) {
                    cacheCount = listview.cacheBuffer
                    listview.cacheBuffer = -1
                }
                else if ( gridView && typeof( gridView ) != undefined) {
                    cacheCount = gridView.cacheBuffer
                    gridView.cacheBuffer = -1
                } else if ( pathview && typeof( pathview ) != undefined) {
                    cacheCount = pathview.cacheBuffer
                }
            } else {
                secDialog.opacity = 0.0;
                //secDialog.visible = false;
                if ( listview && typeof( listview ) != undefined) {
                    listview.cacheBuffer = cacheCount
                } else if ( gridView && typeof( gridView ) != undefined) {
                    gridView.cacheBuffer = cacheCount
                } else if ( typeof( pathview ) != undefined) {
                }
            }
        }
        onMouseYChanged: {
            if(interactive && pressed && !landscape) {
               // secDialog.color = Theme.rgba(Theme.highlightColor,0.5);
                var relPos = (mouseY/height)*100;
                var item = Sections.getSectionNameAtRelativePos(relPos);
                if ( item )  {
                    secDialog.text = item.value;
                    if ( listview )
                        listview.positionViewAtIndex(item.index,ListView.Beginning);
                    else if ( pathview )
                        pathview.positionViewAtIndex(item.index,PathView.Center);
                    else if ( gridView )
                        gridView.positionViewAtIndex(item.index,GridView.Beginning);
                }
            }
        }
        onMouseXChanged: {
            if(interactive && pressed && landscape) {
               // secDialog.color = Theme.rgba(Theme.highlightColor,0.5);
                var relPos = (mouseX/width)*100;
                var item = Sections.getSectionNameAtRelativePos(relPos);
                if ( item ) {
                    secDialog.text = item.value;
                    if ( listview )
                        listview.positionViewAtIndex(item.index,ListView.Beginning);
                    else if ( pathview )
                        pathview.positionViewAtIndex(item.index,PathView.Center);
                    else if ( gridView )
                        gridView.positionViewAtIndex(item.index,GridView.Beginning);
                }
            }
        }
    }

    states: [
        State {
            name: "landscape"
            when: landscape
            PropertyChanges {
                target: scroller
                width: parent.width
                height: 64 * Theme.pixelRatio
                x:0
                y: parent.y+parent.height-height;
                landscape: true
            }
            PropertyChanges {
                target: secDialog
                anchors {
                    right : undefined
                    bottom: parent.top
                }
            }
        },
        State {
            name: "portrait"
            when: !landscape
            PropertyChanges {
                target: scroller
                height: parent.height
                width: 64 * Theme.pixelRatio
                x: parent.x+parent.width-width;
                y: 0
                landscape: false
            }
            PropertyChanges {
                target: secDialog
                anchors {
                    right : parent.left
                    bottom: undefined
                }
            }
        }
    ]
}
