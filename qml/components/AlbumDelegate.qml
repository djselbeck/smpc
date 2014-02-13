import QtQuick 2.1
import Sailfish.Silica 1.0

ListItem {
        //menu: contextMenu
        width: albumGridView.cellWidth
        contentHeight: albumGridView.cellHeight

        Rectangle {
            anchors.fill: parent
            anchors.margins: Theme.paddingSmall
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
            Image {
                id: albumImage
                anchors.fill: parent
                sourceSize.width: width
                sourceSize.height: height
                source: coverURL
                cache: true
                asynchronous: true
                fillMode: Image.PreserveAspectCrop
            }
            Rectangle {
                id: gradientRect
                visible: true //artistImage.source!=""
                anchors {
                    bottom: parent.bottom
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                }
                width: parent.width

                color: Theme.highlightBackgroundColor
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
            albumClicked(artistname, title)
            pageStack.push(Qt.resolvedUrl("../pages/database/AlbumTracksPage.qml"),{artistname:artistname,albumname:title});
        }
//        Component.onCompleted: {
//            console.debug("Album created: " + title)
//        }
//        Component.onDestruction: {
//            console.debug("Album destroyed:" + title)
//        }

//        function playAlbumRemorse() {
//            remorseAction(qsTr("playing album"), function () {
//                playAlbum([artistname, title])
//            }, 3000)
//        }
//        function addAlbumRemorse() {
//            remorseAction(qsTr("adding album"), function () {
//                addAlbum([artistname, title])
//            }, 3000)
//        }
//        Component {
//            id: contextMenu
//            ContextMenu {
//                MenuItem {
//                    text: qsTr("play album")
//                    onClicked: {
//                        if (title !== "") {
//                            playAlbumRemorse()
//                        }
//                    }
//                }

//                MenuItem {
//                    text: qsTr("add album to list")
//                    onClicked: {
//                        if (title !== "") {
//                            addAlbumRemorse()
//                        }
//                    }
//                }
//            }
//        }
    }
