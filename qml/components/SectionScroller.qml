import QtQuick 2.0
import Sailfish.Silica 1.0
import "SectionScroller.js" as Sections

Item {
    id: scroller
    height: parent.height
    width: parent.width/7
    x: parent.x+parent.width-width;
    z:1
    property GridView listview;
    property string sectionPropertyName
//    Rectangle {
//        id: testrect
//        opacity:0.5
//        anchors.fill:parent
//    }

    onListviewChanged: {
        if(listview && listview.model) {
            Sections.fillSections(listview,scroller.sectionPropertyName);
        } else if(listview) {
            listview.modelChanged.connect( function() {
                Sections.fillSections(listview,scroller.sectionPropertyName);
            });

        }
    }

    Rectangle {
        id: secDialog
        property alias text: currSecText.text
        anchors.right:  parent.left
        y:inputArea.mouseY-(height/2) > (parent.height-height) ? (parent.height-height) : (inputArea.mouseY-(height/2) < 0 ? 0 : inputArea.mouseY-(height/2))
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
        preventStealing: true
        onPressedChanged: {
            if ( pressed ) {
                secDialog.color = Theme.rgba(Theme.highlightBackgroundColor,0.5);
                secDialog.opacity = 1.0;
                //secDialog.visible = true;
            } else {
                secDialog.opacity = 0.0;
                //secDialog.visible = false;
            }
        }
        onMouseYChanged: {
            if(pressed) {
               // secDialog.color = Theme.rgba(Theme.highlightColor,0.5);
                var relPos = (mouseY/height)*100;
                var item = Sections.getSectionNameAtRelativePos(relPos);
                secDialog.text = item.value;

                listview.positionViewAtIndex(item.index,ListView.Beginning);
            }
        }
    }


}
