import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

Page {
    SilicaListView {
        anchors.fill: parent
        anchors.margins: theme.paddingMedium
        contentWidth: width
        header: PageHeader {
            title: qsTr("JollaMPC");
        }
        model: mainMenuModel
        delegate: BackgroundItem {
            Label {
                anchors.centerIn: parent
                text: name
            }
            onClicked: {
                parseClicked(ident);
            }
        }
    }


    ListModel {
        id: mainMenuModel
        ListElement { name: "song information"; ident:"currentsong"; icon:"icons/music.svg" }
        ListElement { name: "artists"; ident:"artists" ;icon:"icons/contacts.svg" }
        ListElement { name: "albums"; ident:"albums";icon:"icons/music_album.svg"}
        ListElement { name: "files"; ident:"files" ;icon:"icons/music_file.svg"}
        ListElement { name: "playlist"; ident:"playlist";icon:"icons/playlistlist.svg"}
        ListElement { name: "search"; ident:"search";icon:"icons/search.svg"}
        ListElement { name: "connect"; ident:"connectto";icon:"icons/connectivity.svg"}
        ListElement { name: "settings"; ident:"settings";icon:"icons/settings.svg"}
    }

}


