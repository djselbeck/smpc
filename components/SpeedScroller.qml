import QtQuick 2.0
import Sailfish.Silica 1.0
import "SpeedScroller.js" as Sections

Item {
    id: scroller
    height: parent.height
    width: parent.width/5
    property alias scrollenabled: inputArea.enabled
    x: parent.x+parent.width-width;
    z:1
    property ListView listview;
//    Rectangle {
//        id: testrect
//        opacity:0.5
//        anchors.fill:parent
//    }

    onListviewChanged: {
        console.debug("Listview changed");
        if(listview && listview.model) {
            console.debug("Got listview AND model, initialize section data");
            Sections.fillSections(listview);
            listview.modelChanged.connect( function() {
                Sections.fillSections(listview);
            });
        } else if(listview) {
            console.debug("Got listview connecting modelChanged");
            listview.modelChanged.connect( function() {
                Sections.fillSections(listview);
            });
        }
    }

    MouseArea {
        id: inputArea
        anchors.fill:parent
        preventStealing: true
        onPressed: {
            console.debug("scrollerpos:"+mouseX+":"+mouseY);
        }
        onMouseYChanged: {
            if(pressed) {
                var relPos = (mouseY/height)*100;
                console.debug("pos changed:"
                              + relPos);
                var item = Sections.getItemPositionFromRelative(relPos);
                console.debug("Jump to index:"+ item);
                if(listview && listview.model) {
                    var count = (typeof listview.model.count === 'undefined' ? listview.model.length : listview.model.count);
                    if ( item < count )
                        listview.positionViewAtIndex(item,ListView.Beginning);
                }
            }
        }
    }
}
