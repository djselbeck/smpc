import QtQuick 1.1
import Sailfish.Silica 1.0


Page
{
    id: aboutPage;
    Column
    {
        anchors.fill: parent
        id: contentColumn;
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
