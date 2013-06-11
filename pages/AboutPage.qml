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
        PageHeader {
            title: qsTr("about");
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
            text:"© 2013 by Hendrik Borghorst"
            font.pixelSize: theme.fontSizeMedium
        }
        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text:"Licensed under GPLv3"
            font.pixelSize: theme.fontSizeMedium
        }
        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text:"Fetches covers from last.fm"
            font.pixelSize: theme.fontSizeMedium
        }
    }

}
