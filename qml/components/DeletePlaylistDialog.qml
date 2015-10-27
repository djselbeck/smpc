import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    allowedOrientations: Orientation.All
    Column {
        width: parent.width
        spacing: Theme.paddingMedium
        anchors.margins: Theme.paddingMedium
        DialogHeader {
            acceptText: qsTr("delete playlist")
        }
        Label {
            text: qsTr("really delete playlist?")
        }
    }
    onDone: {
        if (result === DialogResult.Accepted) {
            deletePlaylist()
        }
    }
}
