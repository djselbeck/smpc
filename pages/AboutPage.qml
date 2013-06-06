import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: aboutPage;
    Column
    {
        anchors.fill: parent
        id: contentColumn;
        Heading {
            anchors.right: parent.right
            anchors.left: parent.left
            text: qsTr("about");
        }
        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text:"JollaMPC"
            font.pixelSize: theme.fontSizeExtraLarge
        }
        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text:"Hendrik Borghorst"
            font.pixelSize: theme.fontSizeMedium
        }
        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text:"Licensed under GPLv3"
            font.pixelSize: theme.fontSizeMedium
        }
    }

}
