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
            anchors.fill: parent
            contentWidth: width
            header: PageHeader {
                title: qsTr("artists");
            }
            VerticalScrollDecorator { flickable: artistListView }
            delegate: BackgroundItem {
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
