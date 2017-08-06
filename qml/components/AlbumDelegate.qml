import QtQuick 2.1
import Sailfish.Silica 1.0

ListItem {
        width: albumGridView.cellWidth
        contentHeight: albumGridView.cellHeight

        layer.enabled: true
        layer.effect: ShaderEffect {
            blending: highlighted
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: Theme.paddingSmall
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
            Image {
                id: albumImage
                anchors.fill: parent
                source: albumGridView.scrolling ? "" : coverURL
                cache: false
                asynchronous: true
                fillMode: Image.PreserveAspectCrop
            }
            Rectangle {
                id: gradientRect
                visible: true
                anchors {
                    bottom: parent.bottom
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                }
                width: parent.width

                gradient: Gradient {
                    GradientStop {
                        position: 0.5
                        color: Qt.rgba(0.0, 0.0, 0.0, 0.0)
                    }
                    GradientStop {
                        position: 1.0
                        color: Qt.rgba(0.0, 0.0, 0.0, 0.8)
                    }
                }
            }
            Label {
                anchors {
                    bottom: albumImage.bottom
                    horizontalCenter: albumImage.horizontalCenter
                }
                height: parent.height * 0.5
                width: parent.width
                wrapMode: "WordWrap"
                elide: Text.ElideRight
                font.pixelSize: Theme.fontSizeSmall
                style: Text.Raised
                styleColor: Theme.secondaryColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                text: title === "" ? qsTr("No Album Tag") : title
            }
        }


        onClicked: {
            albumGridView.currentIndex = index
            albumClicked(artist, title)
            pageStack.push(Qt.resolvedUrl("../pages/database/AlbumTracksPage.qml"),{artistname:artist,albumname:title});
        }

    }
