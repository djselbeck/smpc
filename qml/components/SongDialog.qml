import QtQuick 2.0

import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: songDialog
    allowedOrientations: bothOrientation
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

        id: songFlickable
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
                height: ready ? ( orientation === Orientation.Portrait ? parent.width : (songDialog.height-header.height-buttonRow.height ) ) : 0
                width: height
                anchors{
                    horizontalCenter: parent.horizontalCenter
                }

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
                text: qsTr("play after current")
                onClicked: {
                    addSongAfterCurrent(filename)
                    pageStack.navigateBack(PageStackAction.Animated)
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
//            bottomMargin: quickControlPanel.visibleSize
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
//    states: [
//        State {
//            name: "portrait"
//            when: orientation === Orientation.Portrait
//            PropertyChanges {
//                target: buttonRow
//                anchors.bottomMargin: quickControlPanel.visibleSize
//                anchors.rightMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//            PropertyChanges {
//                target: songFlickable
//                anchors.bottomMargin: 0
//                anchors.rightMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        },State {
//            name: "landscape"
//            when: orientation === Orientation.Landscape
//            PropertyChanges {
//                target: buttonRow
//                anchors.bottomMargin: 0
//                anchors.rightMargin: quickControlPanel.visibleSize
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//            PropertyChanges {
//                target: songFlickable
//                anchors.bottomMargin: 0
//                anchors.rightMargin: quickControlPanel.visibleSize
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        }]
}
