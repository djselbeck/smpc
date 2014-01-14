import QtQuick 2.0
import Sailfish.Silica 1.0
import "SectionScroller.js" as Sections

Item {
    id: scroller
//    height: parent.height
//    width: parent.width/7
//    x: parent.x+parent.width-width;
    property GridView listview;
    property PathView pathview;
    property bool interactive:true
    property string sectionPropertyName
    property bool landscape:false
    Rectangle {
        id: testrect
        visible: mDebugEnabled
        opacity:0.5
        anchors.fill:parent
    }

    onListviewChanged: {
        if(listview && listview.model) {
            Sections.fillSections(listview,scroller.sectionPropertyName);
        } else if(listview) {
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

    Rectangle {
        id: secDialog
        property alias text: currSecText.text
        y: landscape ? 0 : (inputArea.mouseY-(height/2) > (parent.height-height) ? (parent.height-height) : (inputArea.mouseY-(height/2) < 0 ? 0 : inputArea.mouseY-(height/2)) )
        x: landscape ?  (inputArea.mouseX-(width/2) > (parent.width-width) ? (parent.width-width) : (inputArea.mouseX-(width/2) < 0 ? 0 : inputArea.mouseX-(width/2)) ) : 0
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
            } else {
                secDialog.opacity = 0.0;
                //secDialog.visible = false;
            }
        }
        onMouseYChanged: {
            if(interactive && pressed && !landscape) {
               // secDialog.color = Theme.rgba(Theme.highlightColor,0.5);
                var relPos = (mouseY/height)*100;
                var item = Sections.getSectionNameAtRelativePos(relPos);
                if ( item) {
                    secDialog.text = item.value;
                    if ( listview )
                        listview.positionViewAtIndex(item.index,GridView.Beginning);
                    if ( pathview )
                        pathview.positionViewAtIndex(item.index,PathView.Center);
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
                        listview.positionViewAtIndex(item.index,GridView.Beginning);
                    if ( pathview ) {
                        console.debug("positioning view at: " + item.index)
                        pathview.positionViewAtIndex(item.index,PathView.Center);
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "landscape"
            PropertyChanges {
                target: scroller
                width: parent.width
                height: parent.height/7
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
            PropertyChanges {
                target: scroller
                height: parent.height
                width: parent.width/7
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
    state: (orientation === Orientation.Portrait || orientation === Orientation.PortraitInverted  ) ? "portrait" : "landscape"

}
