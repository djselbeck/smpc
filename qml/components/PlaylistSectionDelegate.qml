import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    height: Theme.itemSizeMedium
    width: parent.width
    property string album: section.split('|')[1]
    property string artist: section.split('|')[0]
    Rectangle {
        id: sectionFillRect
        color: Theme.rgba(Theme.highlightBackgroundColor,
                          0.2) //Qt.rgba(0.0, 0.0, 0.0, 0.2)
        anchors {
            fill: parent
        }
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.0)
            }
            GradientStop {
                position: 1.0
                //color: Theme.rgba(Theme.highlightBackgroundColor, 0.6)
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.3)
            }
        }
    }

    Label {
        id: sectionLabel
        height: parent.height
        text: album
        anchors {
            left: parent.left
            leftMargin: listPadding
            right: sectionImage.left
            rightMargin: Theme.paddingSmall
        }

        font.pixelSize: Theme.fontSizeLarge
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        maximumLineCount: 1
        elide: Text.ElideRight
    }
    Image {
        id: sectionImage
        height: parent.height
        width: height
        anchors {
            right: parent.right
            rightMargin: listPadding
        }
        cache: true
        asynchronous: true
        sourceSize.height: height
        sourceSize.width: width
        fillMode: Image.PreserveAspectFit
        source: "image://imagedbprovider/album/" + artist + "/" + album
        Image {
            id: dummyImage
            anchors.fill: parent
            visible: (parent.status !== Image.Ready)
            source: "image://theme/icon-l-music"
        }
    }
    OpacityRampEffect {
        sourceItem: sectionImage
        direction: OpacityRamp.BottomToTop
    }
}
