import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: playlistTracksPage
    property alias listmodel: playlistTracksListView.model;
    property string playlistname;
    SilicaListView {
            id : playlistTracksListView
            anchors.fill: parent
            contentWidth: width
            header: PageHeader {
                title: playlistname;
            }
            PullDownMenu {
                MenuItem {
                    text: qsTr("delete list")
                    onClicked: {
                        deleteSavedPlaylistQuestion.playlistname = playlistname;
                        pageStack.openDialog(deleteSavedPlaylistQuestion);
                    }
             }
                MenuItem {
                    text: qsTr("add list")
                    onClicked: {
                        addPlaylist(playlistname)
                    }
             }
            }
            delegate: BackgroundItem {

                Column{
                    x : theme.paddingLarge
                    anchors.verticalCenter: parent.verticalCenter
                        Row{
                            Label {text: (index+1)+". ";anchors {verticalCenter: parent.verticalCenter}}
                            Label {clip: true; wrapMode: Text.WrapAnywhere; elide: Text.ElideRight; text:  (title==="" ? filename : title);anchors {verticalCenter: parent.verticalCenter}}
                            Label { text: (length===0 ? "": " ("+lengthformated+")");anchors {verticalCenter: parent.verticalCenter}}
                        }
                        Label{
                            text:(artist!=="" ? artist + " - " : "" )+(album!=="" ? album : "");
                            color: theme.secondaryColor;
                            font.pixelSize: theme.fontSizeSmall
                        }
                    }
                onClicked: {
                    albumTrackClicked(title,album,artist,lengthformated,uri,year,tracknr);
                }
            }
    }
    Dialog
    {
        id: deleteSavedPlaylistQuestion
        property string playlistname;
        Column {
            width: parent.width
            spacing: 10
            anchors.margins: theme.paddingMedium
            DialogHeader {
                acceptText: qsTr("delete playlist");
            }
            Label {
                text: qsTr("really delete playlist?")
             }
        }
        onDone: {
            if ( result == DialogResult.Accepted)
            {
                deleteSavedPlaylist(playlistname);
                pageStack.clear();
                pageStack.push(initialPage);
            }
        }
    }

}
