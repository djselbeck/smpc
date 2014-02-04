import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page
{
    id: filePage
    allowedOrientations: bothOrientation
    property string filepath;
    property var listmodel;
    property int lastContentPosY:0
    SilicaListView {
            id : fileListView
            model: listmodel
            highlightFollowsCurrentItem: true
            SpeedScroller{
                id: scroller
                listview: fileListView
                scrollenabled: fastscrollenabled
            }
            clip:true
            ScrollDecorator {}

            anchors {
                fill: parent
//                bottomMargin: quickControlPanel.visibleSize
            }
            contentWidth: width
            header: PageHeader {
                title: qsTr("filebrowser");//(filepath===""? "Files:" : filepath)
            }
            PullDownMenu {
                MenuItem {
                    text: qsTr("home")
                    onClicked: {
                        pageStack.clear();
                        pageStack.push(initialPage);
                    }
             }
                MenuItem {
                    text: qsTr("add folder")
                    onClicked: {
                        addFiles(filepath);
                    }
             }
                MenuItem {
                    text: qsTr("play folder")
                    onClicked: {
                        playFiles(filepath);
                    }
             }
            }
            delegate: FileDelegate {}
    }

    onStatusChanged: {
        // Restore scroll position
        if ( status === PageStatus.Activating && lastContentPosY) {
            fileListView.cancelFlick();
            fileListView.positionViewAtIndex(lastContentPosY,ListView.Center);
        }
    }

    Component.onDestruction: {
        console.debug("popping file model stack");
        fastscrollenabled = false;
        popfilemodelstack();
    }

    Component.onCompleted: {
        console.debug("filepage created: ");
    }

//    states: [
//        State {
//            name: "portrait"
//            when: orientation === Orientation.Portrait
//            PropertyChanges {
//                target: fileListView
//                anchors.bottomMargin: quickControlPanel.visibleSize
//                anchors.rightMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        },State {
//            name: "landscape"
//            when: orientation === Orientation.Landscape
//            PropertyChanges {
//                target: fileListView
//                anchors.bottomMargin: 0
//                anchors.rightMargin: quickControlPanel.visibleSize
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        }]

}
