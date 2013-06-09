import QtQuick 1.1
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
//            PullDownMenu {
//                MenuItem {
//                    text: qsTr("add albums")
//                    visible: artist === "" ? false : true;
//                    onClicked: {
//                        addArtist(artistname);
//                    }
//             }
//            }
            delegate: BackgroundItem {
                Column{
                    x : theme.paddingLarge
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


}
