import QtQuick 2.0

import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: currentsong_page
    property string title
    property string album
    property string artist
    property alias lengthtext: lengthText.text
    property alias nr: nrText.text
    property alias date: dateText.text
    property alias filename: fileText.text
    property int fontsize: Theme.fontSizeMedium
    property int fontsizegrey: Theme.fontSizeSmall

    SilicaFlickable {

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: buttonRow.top
        }
        contentHeight: infocolumn.height
        clip: true
        Column {
            clip: true
            anchors {
                right: parent.right
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: listPadding
                rightMargin: listPadding
            }
            id: infocolumn
            DialogHeader {
                id: header
                acceptText: qsTr("add song")
            }
            ToggleImage{
                height: parent.width
                width: height
                sourceprimary:  "image://imagedbprovider/album/" + artist +"/" + album
                sourcesecondary: "image://imagedbprovider/artist/" + artist;
                fillMode: Image.PreserveAspectFit
            }

            ScrollLabel {
                id: titleText
                text: title
                color: Theme.primaryColor
                font.pixelSize: fontsize
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            ScrollLabel {
                id: albumText
                text: album
                color: Theme.primaryColor
                font.pixelSize: fontsize
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            ScrollLabel {
                id: artistText
                text: artist
                color: Theme.primaryColor
                font.pixelSize: fontsize
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            Label {
                text: qsTr("length:")
                color: Theme.secondaryColor
                font.pixelSize: fontsizegrey
            }
            Label {
                id: lengthText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                wrapMode: "WordWrap"
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            Label {
                text: qsTr("date:")
                color: Theme.secondaryColor
                font.pixelSize: fontsizegrey
            }
            Label {
                id: dateText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                wrapMode: "WordWrap"
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            Label {
                text: qsTr("nr.:")
                color: Theme.secondaryColor
                font.pixelSize: fontsizegrey
            }
            Label {
                id: nrText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                wrapMode: "WordWrap"
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            Label {
                text: qsTr("uri:")
                color: Theme.secondaryColor
                font.pixelSize: fontsizegrey
            }
            Label {
                id: fileText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                wrapMode: "WrapAnywhere"
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
        }
        PullDownMenu {
            MenuItem {
                text: qsTr("add song")
                onClicked: {
                    accept()
                }
            }
            MenuItem {
                text: qsTr("play song")
                onClicked: {
                    playSong(filename)
                    pageStack.navigateBack(PageStackAction.Animated)
                }
            }
        }
    }

    Row {
        id: buttonRow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors {
            bottom: parent.bottom /*right:parent.right;left:parent.left*/
        }
        IconButton {
            id: playButton
            icon.source: "image://theme/icon-m-play"
            onClicked: {
                playSong(filename)
                pageStack.pop()
            }
        }
        IconButton {
            id: addButton
            icon.source: "image://theme/icon-m-add"
            onClicked: {
                accept()
            }
        }
    }

    onAccepted: {
        addSong(filename)
    }
}
