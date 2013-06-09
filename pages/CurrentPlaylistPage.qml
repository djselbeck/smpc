import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: currentPlaylistPage
    property alias listmodel: playlistView.model;
    property int songid;
    SilicaListView {
            id : playlistView
            anchors.fill: parent
            anchors.margins: theme.paddingLarge
            contentWidth: width
            header: PageHeader {
                title: qsTr("playlist");
            }
            PushUpMenu {
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
            }
            delegate: BackgroundItem {
                Column{
                    anchors.verticalCenter: parent.verticalCenter
                        Row{
                            Label {text: (index+1)+". ";anchors {verticalCenter: parent.verticalCenter}}
                            Label {clip: true; wrapMode: Text.WrapAnywhere; elide: Text.ElideRight; text:  (title==="" ? filename : title);font.italic:(playing) ? true:false;anchors {verticalCenter: parent.verticalCenter}}
                            Label { text: (length===0 ? "": " ("+lengthformated+")");anchors {verticalCenter: parent.verticalCenter}}
                        }
                        Label{
                            text:(artist!=="" ? artist + " - " : "" )+(album!=="" ? album : "");
                            color: theme.secondaryColor;
                            font.pixelSize: theme.fontSizeSmall
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
        sourceComponent: Column {
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
                deletePlaylist();
            }
        }
    }

    Dialog
    {
        id: saveplaylistDialog
        sourceComponent: Column {
            width: parent.width
            spacing: 10
            anchors.margins: theme.paddingMedium
        DialogHeader {
            acceptText: qsTr("save playlist");
        }
        Label {
            text: qsTr("playlist name:")
         }
        TextField
        {
            id: playlistNameField
            placeholderText: qsTr("input playlist name")
        }
        }
        onDone: {
            if ( result == DialogResult.Accepted)
            {
                savePlaylist(playlistNameField.text);
            }
            playlistNameField.text="";
        }
    }

}
