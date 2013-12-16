import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: filePage
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

            anchors.fill: parent
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

}
