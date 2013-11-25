import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: filePage
    property string filepath;
    property alias listmodel: fileListView.model;
    SilicaListView {
            id : fileListView
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
            delegate: ListItem {
            menu: contextMenu
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
                    visible: isFile
                    text: ( !isFile ? "" : (title==="" ?"" : title+ " - ") + (artist==="" ?  "" : artist) );
                    anchors {left: parent.left;right:parent.right;}
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.secondaryColor
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
                if(isPlaylist) {
                    console.debug("Playlist:"+(prepath=="/"? "": prepath+"/")+name);
                    savedPlaylistClicked((prepath=="/"? "": prepath+"/")+name);
                }
            }

            function addTrackRemorse() {
                remorseAction(qsTr("adding track"), function() { addSong(path); },3000)
            }
            function addPlaylistRemorse() {
                remorseAction(qsTr("adding playlist"), function() { console.debug("playlist: " + (prepath=="/"? "": prepath+"/")+name + " requested");
                    addPlaylist((prepath=="/"? "": prepath+"/")+name); },3000)
            }
            function addFolderRemorse() {
                remorseAction(qsTr("adding directory"), function() { addFiles((prepath=="/"? "": prepath+"/")+name); },3000)
            }
            function playTrackRemorse() {
                remorseAction(qsTr("playing track"), function() { playSong(path); },3000)
            }
            function playPlaylistRemorse() {
                remorseAction(qsTr("playing playlist"), function() { playPlaylist((prepath=="/"? "": prepath+"/")+name); },3000)
            }
            function playFolderRemorse() {
                remorseAction(qsTr("playing directory"), function() { playFiles((prepath=="/"? "": prepath+"/")+name); },3000)
            }
            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        text: isFile ? qsTr("play file") : ( isDirectory ? qsTr("play directory") : qsTr("play playlist") );
                        onClicked: {
                            if(isFile) {
                                playTrackRemorse();
                            }
                            else if (isDirectory) {
                                playFolderRemorse();
                            }
                            else if (isPlaylist) {
                                playPlaylistRemorse();
                            }
                        }
                    }

                    MenuItem {
                        text: isFile ? qsTr("add file") : ( isDirectory ? qsTr("add directory") : qsTr("add playlist") );
                        onClicked: {
                            if(isFile) {
                                addTrackRemorse();
                            }
                            else if (isDirectory) {
                                addFolderRemorse();
                            }
                            else if (isPlaylist) {
                                addPlaylistRemorse();
                            }
                        }
                    }
                }
            }

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
