import QtQuick 2.0
import Sailfish.Silica 1.0


DockedPanel {
    id: controlPanel

    open: !hideControl && !Qt.inputMethod.visible
    width: parent.width
    height: textColumn.height + Theme.paddingMedium
    contentHeight: height

    property bool hideControl: false

    flickableDirection: Flickable.VerticalFlick


    Image {
            width: parent.width
            fillMode: Image.PreserveAspectFit
            source: "image://theme/graphic-gradient-edge"
        }

    Label {
        id: notPlayingLabel
        visible: (mTitle=="" && mArtist=="")
        text: qsTr("not playing")
        anchors.centerIn: parent
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeLarge
        font.bold: false
        font.family: Theme.fontFamily

    }

    Column {
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
        }

        id: textColumn
        ScrollLabel {
            id: titleText
            text: mTitle
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
            font.bold: false
            font.family: Theme.fontFamily
            anchors {
                left: parent.left
                right: parent.right
            }
             active: controlPanel.open && Qt.application.active
        }
        ScrollLabel {
            id: artistText
            text: mArtist
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
            font.bold: false
            font.family: Theme.fontFamily
            anchors {
                left: parent.left
                right: parent.right
            }
            active: controlPanel.open && Qt.application.active
        }
    }

    PushUpMenu {
        id: pushUp
        Row {
            id:buttonRow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: Theme.paddingMedium
            height: shuffleButton.height
            Switch {
                id: shuffleButton
                automaticCheck: false
                icon.source: "image://theme/icon-m-shuffle"
                checked: mShuffle
                onClicked: {
                    setShuffle(!checked)
                }
            }
            IconButton {
                id: prevButton
                icon.source: "image://theme/icon-m-previous"
                onClicked: prev()
            }
            IconButton {
                id: stopButton
                icon.source: "qrc:images/icon-m-stop.png"
                onClicked: stop()
            }
            IconButton {
                id: playButton
                icon.source: playbuttoniconsource
                onClicked: play()
            }
            IconButton {
                id: nextButton
                icon.source: "image://theme/icon-m-next"
                onClicked: next()
            }
            Switch {
                id: repeatButton
                automaticCheck: false
                checked: mRepeat
                icon.source: "image://theme/icon-m-repeat"
                onClicked: {
                    setRepeat(!checked)
                }
            }
        }

        Slider {
            id: volumeSlider
            width:parent.width
            stepSize: 1
            maximumValue: 100
            minimumValue: 0
            value: mVolume
            valueText: value + "%"
            label: qsTr("volume")
            onPressedChanged: {
                if (!pressed) {
                    volumeChanging = false
                    setVolume(value)
                    value  = Qt.binding(function() {return mVolume;});
                } else {
                    volumeChanging = true
                }
            }
            onValueChanged: {
                if(pressed)
                    setVolume(value)
                // valueText = value+"%";
            }
        }

    }


}
