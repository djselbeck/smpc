import QtQuick 2.0
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
            text:"SMPC"
            font.pixelSize: Theme.fontSizeExtraLarge
        }
        Button
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text:"Homepage"
            onClicked:
            {
                Qt.openUrlExternally('https://github.com/djselbeck/smpc');
            }

        }
        Separator
        {
            anchors.horizontalCenter: parent.horizontalCenter
            width:parent.width-Theme.paddingLarge
            color: Theme.primaryColor
        }
        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text:"© 2013 by Hendrik Borghorst"
            font.pixelSize: Theme.fontSizeMedium
        }

        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text:"Licensed under GPLv3"
            font.pixelSize: Theme.fontSizeMedium
        }
        Separator
        {
            anchors.horizontalCenter: parent.horizontalCenter
            width:parent.width-Theme.paddingLarge
            color: Theme.primaryColor
        }
        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text:"Fetches covers from last.fm"
            font.pixelSize: Theme.fontSizeTiny
        }
    }

}
