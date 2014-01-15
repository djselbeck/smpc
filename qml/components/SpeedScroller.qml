import QtQuick 2.0
import Sailfish.Silica 1.0
import "SpeedScroller.js" as Sections

Item {
    id: scroller
    height: parent.height
    width: parent.width/7
    property alias scrollenabled: inputArea.enabled
    property int listviewCache
    x: parent.x+parent.width-width;
    z:1
    property ListView listview;
//    Rectangle {
//        id: testrect
//        opacity:0.5
//        anchors.fill:parent
//    }

    onListviewChanged: {
        if(listview && listview.model) {
            Sections.fillSections(listview);
            listview.modelChanged.connect( function() {
                Sections.fillSections(listview);
            });
        } else if(listview) {
            listview.modelChanged.connect( function() {
                Sections.fillSections(listview);
            });
        }
    }

    MouseArea {
        id: inputArea
        anchors.fill:parent
        preventStealing: true
        onMouseYChanged: {
            if(pressed) {
                listviewCache = listview.cacheBuffer
                listview.cacheBuffer = 0
                var relPos = (mouseY/height)*100;
                var item = Sections.getItemPositionFromRelative(relPos);
                if(listview && listview.model) {
                    var count = (typeof listview.model.count === 'undefined' ? listview.model.length : listview.model.count);
                    if ( item < count )
                        listview.positionViewAtIndex(item,ListView.Beginning);
                }
            } else {
                // Restore cacheBuffer
                listview.cacheBuffer = cacheBuffer
            }
        }
    }
}
