import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: filePage
    property string filepath;
    property variant model;
    property alias listmodel: fileListView.model;
    SilicaListView {
            id : fileListView
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
            delegate: BackgroundItem {
            id:filesDelegate
                Image {
                    id: fileicon
                    source: (isDirectory===true ? "image://theme/icon-l-storage": "image://theme/icon-m-music");
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                }
                Column{
                    anchors{left: fileicon.right;right:parent.right;verticalCenter:parent.verticalCenter;}
                Label{
                    id:filenametext
                    text: name
                    wrapMode: "NoWrap"
                    anchors {left: parent.left;right:parent.right}
                    elide: Text.ElideMiddle;
                }
                Label
                {
                    visible: isDirectory===false
                    text: (isDirectory===true ? "" : (title==="" ?"" : title+ " - ") + (artist==="" ?  "" : artist) );
                    anchors {left: parent.left;right:parent.right;}
                }
                }
            onClicked: {
                if(isDirectory){
                    filesClicked((prepath=="/"? "": prepath+"/")+name);
                }
                if(isFile) {
                    console.debug("Album:"+album)
                    albumTrackClicked(title,album,artist,length,path,year,tracknr);
                }
            }

        }
    }

    Component.onDestruction: {
        console.debug("popping file model stack");
        popfilemodelstack();
    }

}
