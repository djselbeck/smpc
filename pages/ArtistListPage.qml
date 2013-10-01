import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: artistlistPage
    property alias listmodel: artistListView.model;
    property string artistname;
    SilicaListView {
            id : artistListView
            ScrollDecorator {}
            anchors.fill: parent
            contentWidth: width
            header: PageHeader {
                title: qsTr("artists");
            }
            VerticalScrollDecorator { flickable: artistListView }
            delegate: ListItem {
                menu: contextMenu
                Column{
                    x : Theme.paddingLarge
                    anchors.verticalCenter: parent.verticalCenter
                         Label{
                             text: (artist===""? "No Artist Tag": artist)
                        }
                    }
                onClicked: {
                    artistClicked(artist);
                }
                function playArtistRemorse() {
                    remorseAction(qsTr("playing artist"), function() { playArtist(artist); },3000)
                }
                function addArtistRemorse() {
                    remorseAction(qsTr("adding artist"), function() { addArtist(artist); },3000)
                }
                Component {
                            id: contextMenu
                            ContextMenu {
                                MenuItem {
                                    text: qsTr("play artist");
                                    onClicked: {
                                        if(artist !=="") {
                                            playArtistRemorse();
                                        }
                                    }
                                }

                                MenuItem {
                                    text: qsTr("add artist to list")
                                    onClicked: {
                                        if(artist !=="") {
                                            addArtistRemorse();
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

    function artistClicked(item)
    {
        mainWindow.artistname = item;
        requestArtistAlbums(item);
    }

}
