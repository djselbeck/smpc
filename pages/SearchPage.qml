import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"


Page{
    id: searchpage
    property alias listmodel:albumsongs_list_view.model
    property int currentindex:-1;
    property string selectedsearch;

    SilicaFlickable
    {
        id: mainSearchFlickable
        contentHeight: albumsongs_list_view.contentHeight + searchhead.height
        anchors.fill: parent
        ScrollDecorator {}
        Column{
            id: searchhead
            spacing: Theme.paddingMedium
            anchors {top:parent.top;left:parent.left;right:parent.right}
            PageHeader {
                title: qsTr("search");
                width: parent.width - Theme.paddingMedium
            }
            TextField {
                id: searchfield
                width: parent.width - Theme.paddingMedium
                text: ""
                inputMethodHints: Qt.ImhNoPredictiveText

             }
            Button{
                id:startsearchbtn
                text: qsTr("Search")
                width: parent.width - Theme.paddingMedium
                onClicked: {
                    var searchfor;
                    switch (searchforcombobox.currentIndex) {
                    case 0 :
                        console.debug( "Searching for titles" );
                        searchfor = "title";
                        break;
                    case 1 :
                        console.debug( "Searching for albums" );
                        searchfor = "album";
                        break;
                    case 2 :
                        console.debug( "Searching for artists" );
                        searchfor = "artist";
                        break;
                    case 3 :
                        console.debug( "Searching for files" );
                        searchfor = "file";
                        break;
                    }

                    requestSearch([searchfor,searchfield.text]);
                    albumsongs_list_view.forceActiveFocus();

                }

            }
            ComboBox{
                id: searchforcombobox
                label: "Search for:"
                width: parent.width - Theme.paddingMedium
                menu: ContextMenu {
                        MenuItem { text: "Titles" }
                        MenuItem { text: "Albums" }
                        MenuItem { text: "Artists" }
                        MenuItem { text: "Files" }
                }
            }
        }

        SilicaListView{
            id: albumsongs_list_view
            contentHeight: 0
            height: contentHeight

            onContentHeightChanged: {
//                height = contentHeight;
                console.debug("New contentHeight:" + contentHeight + " height: " + height);
            }
            interactive: false

            delegate:ListItem {
                menu: contextMenu
                Column{
                    x : Theme.paddingLarge
                    anchors.verticalCenter: parent.verticalCenter
                        Row{
                            Label {text: (index+1)+". ";anchors {verticalCenter: parent.verticalCenter}}
                            Label {clip: true; wrapMode: Text.WrapAnywhere; elide: Text.ElideRight; text:  (title==="" ? filename : title);anchors {verticalCenter: parent.verticalCenter}}
                            Label { text: (length===0 ? "": " ("+lengthformated+")");anchors {verticalCenter: parent.verticalCenter}}
                        }
                        Label{
                            text:(artist!=="" ? artist + " - " : "" )+(album!=="" ? album : "");
                            color: Theme.secondaryColor;
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                onClicked: {
                    albumTrackClicked(title,album,artist,lengthformated,uri,year,tracknr);
                }
                function playTrackRemorse() {
                    remorseAction(qsTr("playing track"), function() { playSong(uri); },3000)
                }
                function addTrackRemorse() {
                    remorseAction(qsTr("adding track"), function() { addSong(uri); },3000)
                }
                Component {
                    id: contextMenu
                    ContextMenu {
                        MenuItem {
                            text: qsTr("play track");
                            onClicked: {
                                playTrackRemorse();
                            }
                        }

                        MenuItem {
                            text: qsTr("add track to list")
                            onClicked: {
                                addTrackRemorse();
                            }
                        }
                    }
                }
            }
            anchors { left: parent.left; right: parent.right; top: searchhead.bottom}//; bottom: parent.bottom }
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("add all results")
                onClicked: {
                    deletePlaylist();
                    addlastsearch();
                }
         }
            MenuItem {
                text: qsTr("play all results")
                onClicked: {
                    deletePlaylist();
                    addlastsearch();
                    playPlaylistTrack(0);
                }
         }

        }

    }


}

