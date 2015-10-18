import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page {
    id: currentSongLarge_page

    allowedOrientations: bothOrientation
    Item {
        id: landscapeImageRow
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: width / 2
        clip: true
//        color: "red"
        Image {
            id: albumImgLandscape
            source: coverimageurl
            width: (parent.width / 2)
            height: width
            anchors {
                top: parent.top
                left: parent.left
            }
            cache: false
            fillMode: Image.PreserveAspectCrop
            Rectangle {
                color: Theme.rgba(
                           Theme.highlightBackgroundColor,
                           Theme.highlightBackgroundOpacity)
                anchors.fill: parent
                visible: albumImgLandscape.status != Image.Ready
                Image {
                    anchors.fill: parent
                    source: "qrc:images/pictogram.svg"
                    sourceSize.width: Screen.width / 2
                    sourceSize.height: Screen.width / 2
                }
            }
        }
        Image {
            id: artistImgLandscape
            source: artistimageurl
            width: (parent.width / 2)
            height: width
            anchors {
                top: parent.top
                left: albumImgLandscape.right
                leftMargin: Theme.paddingSmall
            }
            cache: false
            fillMode: Image.PreserveAspectCrop
            Rectangle {
                color: Theme.rgba(
                           Theme.highlightBackgroundColor,
                           Theme.highlightBackgroundOpacity)
                anchors.fill: parent
                visible: artistImgLandscape.status != Image.Ready
                Image {
                    anchors.fill: parent
                    source: "qrc:images/pictogram.svg"
                    sourceSize.width: Screen.width / 2
                    sourceSize.height: Screen.width / 2
                }
            }
        }
//        Rectangle {
//            anchors.fill: landscapeImageRow
//            gradient: Gradient {
//                GradientStop {
//                    position: 0.5
//                    color: Qt.rgba(0.0, 0.0, 0.0, 0.0)
//                }
//                GradientStop {
//                    position: 0.7
//                    color: Qt.rgba(0.0, 0.0, 0.0, 0.3)
//                }
//                GradientStop {
//                    position: 1.0
//                    color: Qt.rgba(0.0, 0.0, 0.0, 1.0)
//                }
//            }
//        }


    }
    Column {
        id: landscapeTextScrollColumn
        anchors {
            bottom: currentSongLarge_page.bottom
        }
        width: currentSongLarge_page.width

        ScrollLabel {
            id: titleTextLC
            text: mTitle
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeMedium
            width: parent.width
            anchors {
                left: parent.left
                right: parent.right
            }
        }
        ScrollLabel {
            id: albumTextLC
            text: mAlbum
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeMedium
            width: parent.width
            anchors {
                left: parent.left
                right: parent.right
            }
        }
        ScrollLabel {
            id: artistTextLC
            text: mArtist
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeMedium
            width: parent.width
            anchors {
                left: parent.left
                right: parent.right
            }
        }
    }
    onStatusChanged: {
        if (status === PageStatus.Activating || status === PageStatus.Active) {
            quickControlPanel.hideControl = true
        } else if ( status === PageStatus.Deactivating ){
            quickControlPanel.hideControl = false
        }
    }

}
