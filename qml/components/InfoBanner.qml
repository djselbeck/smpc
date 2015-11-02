import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    property alias text: textLabel.text
    width: parent.width
    height: textLabel.contentHeight

    color: Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)

    Label {
        id: textLabel
        width: parent.width
        horizontalAlignment: Text.AlignHCenter

    }
}
