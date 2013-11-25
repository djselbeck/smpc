import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: albumslistPage
    property alias listmodel: albumListView.model;
    property string artistname;
    SilicaListView {
            id : albumListView
            SectionScroller {
                listview: albumListView
            }

            anchors.fill: parent
            contentWidth: width
            header: PageHeader {
                title: artistname == "" ? qsTr("albums") : artistname;
            }
            PullDownMenu {
                MenuItem {
                    text: qsTr("add albums")
                    visible: artistname === "" ? false : true;
                    onClicked: {
                        addArtist(artistname);
                    }
             }
                MenuItem {
                    text: qsTr("play albums")
                    visible: artistname === "" ? false : true;
                    onClicked: {
                        playArtist(artistname);
                    }
             }
            }
            delegate: ListItem {
                menu: contextMenu
                Column{
                    x : Theme.paddingLarge
                    anchors.verticalCenter: parent.verticalCenter
                         Label{
                             text: (title==="" ? qsTr("no album tag") : title)
                        }
                    }
                onClicked: {
                    albumClicked(artistname,title);
                }
                function playAlbumRemorse() {
                    remorseAction(qsTr("playing album"), function() { playAlbum([artistname,title]); },3000)
                }
                function addAlbumRemorse() {
                    remorseAction(qsTr("adding album"), function() { addAlbum([artistname,title]); },3000)
                }
                Component {
                            id: contextMenu
                            ContextMenu {
                                MenuItem {
                                    text: qsTr("play album");
                                    onClicked: {
                                        if(title !=="") {
                                            playAlbumRemorse();
                                        }
                                    }
                                }

                                MenuItem {
                                    text: qsTr("add album to list")
                                    onClicked: {
                                        if(title !=="") {
                                            addAlbumRemorse();
                                        }
                                    }
                                }
                            }
                }
            }
            section {
                property: 'sectionprop'
                delegate: SectionHeader {
                    text: section
                }
            }
    }

}
