import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    allowedOrientations: Orientation.All
    Column {
        width: parent.width
        spacing: Theme.paddingMedium
        anchors.margins: Theme.paddingMedium
        DialogHeader {
            acceptText: qsTr("save playlist")
        }
        Label {
            text: qsTr("playlist name:")
        }
        TextField {
            id: playlistNameField
            width: parent.width
            placeholderText: qsTr("input playlist name")
        }
    }
    onDone: {
        if (result === DialogResult.Accepted) {
            savePlaylist(playlistNameField.text)
        }
        playlistNameField.text = ""
        playlistNameField.focus = false
    }
    onOpened: {
        playlistNameField.focus = true
    }
}
