import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: currentPlaylistPage
    property alias listmodel: playlistView.model;
    property alias songid: playlistView.currentIndex
    SilicaListView {
        id : playlistView
        anchors.fill: parent
        contentWidth: width
        header: PageHeader {
            title: qsTr("playlist");
        }
        PullDownMenu {
            MenuItem {
                text: qsTr("delete playlist")
                onClicked: {
                    pageStack.openDialog(deleteQuestionDialog);
                }
            }
            MenuItem {
                text: qsTr("save playlist")
                onClicked: {
                    pageStack.openDialog(saveplaylistDialog);
                }
            }
            MenuItem {
                text: qsTr("open playlist")
                onClicked: {
                    requestSavedPlaylists();
                }
            }
        }
        delegate: BackgroundItem {

            Column{
                x : Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                Row{
                    Label {text: (index+1)+". ";anchors {verticalCenter: parent.verticalCenter}}
                    Label {clip: true; wrapMode: Text.WrapAnywhere; elide: Text.ElideRight; text:  (title==="" ? filename : title);font.italic:(playing) ? true:false;anchors {verticalCenter: parent.verticalCenter}}
                    Label { text: (length===0 ? "": " ("+lengthformated+")");anchors {verticalCenter: parent.verticalCenter}}
                }
                Label{
                    text:(artist!=="" ? artist + " - " : "" )+(album!=="" ? album : "");
                    color: Theme.secondaryColor;
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
            GlassItem
            {
                anchors.fill: parent
                visible: playing
                scale: 0.5
            }
            onClicked: {
                if(!playing)
                {
                    parseClickedPlaylist(index);
                }
                else{
                    pageStack.push(currentsongpage);
                }
            }
        }
    }

    // Delete question
    Dialog
    {
        id: deleteQuestionDialog
        Column {
            width: parent.width
            spacing: 10
            anchors.margins: Theme.paddingMedium
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
                deletePlaylist();
            }
        }
    }

    Dialog
    {
        id: saveplaylistDialog
        Column {
            width: parent.width
            spacing: 10
            anchors.margins: Theme.paddingMedium
            DialogHeader {
                acceptText: qsTr("save playlist");
            }
            Label {
                text: qsTr("playlist name:")
            }
            TextField
            {
                id: playlistNameField
                width: parent.width
                placeholderText: qsTr("input playlist name")
            }
        }
        onDone: {
            if ( result == DialogResult.Accepted)
            {
                console.debug("Saving playlist: " + playlistNameField.text);
                savePlaylist(playlistNameField.text);
            }
            playlistNameField.text = "";
        }
        onOpened: {
            playlistNameField.focus = true;
        }
    }


    function parseClickedPlaylist(index)
    {
        playPlaylistTrack(index);
    }

}
