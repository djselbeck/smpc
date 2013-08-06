import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: currentsong_page
    property alias title: titleText.text;
    property alias album: albumText.text;
    property alias artist: artistText.text;
    property alias lengthtext:lengthText.text;
    property alias nr: nrText.text;
    property alias date: dateText.text;
    property alias filename: fileText.text;
    property int fontsize:Theme.fontSizeMedium;
    property int fontsizegrey:Theme.fontSizeSmall;

        SilicaFlickable{
            anchors.margins: Theme.paddingMedium
            anchors {top: parent.top;left:parent.left;right:parent.right;bottom:buttonRow.top}
            contentHeight: infocolumn.height
            clip: true
            Column {
                //anchors.fill: parent
                id: infocolumn
                PageHeader {
                    title: qsTr("current song");
                }
                anchors {left:parent.left; right: parent.right;}
                Label{text: qsTr("title:");color:Theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:titleText ;text: "";color:Theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("album:");color:Theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:albumText ;text: "";color:Theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("artist:");color:Theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:artistText ;text: "";color:Theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("length:");color:Theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:lengthText ;text: "";color:Theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("date:");color:Theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:dateText ;text: "";color:Theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("nr.:");color:Theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:nrText ;text: "";color:Theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("uri:");color:Theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:fileText ;text: "";color:Theme.primaryColor; font.pixelSize:fontsize;wrapMode:"WrapAnywhere" ;anchors {left:parent.left; right: parent.right;}}
                clip: true;
            }
        }


        Row
        {
            id: buttonRow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors {bottom:parent.bottom;/*right:parent.right;left:parent.left*/}
            IconButton
            {
                id: playButton
                icon.source: "image://theme/icon-m-play"
                onClicked: {
                    playSong(filename);
                    pageStack.pop();
                }
            }
            IconButton
            {
                id: addButton
                icon.source: "image://theme/icon-m-add"
                onClicked: {
                    addSong(filename);
                    pageStack.pop();
                }
            }


        }

}
