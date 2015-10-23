import QtQuick 2.0
import Sailfish.Silica 1.0

Component {
    ListItem {
        //        menu: contextMenu
        width: GridView.view.cellWidth
        //        menu: contextMenu
        contentHeight: width
        Rectangle {
            anchors.fill: parent
            anchors.margins: Theme.paddingSmall
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
            Image {
                id: artistImage
                anchors.fill: parent
                /*sourceSize.width: width
                sourceSize.height: height*/
                source: imageURL
                cache: false
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
                    bottom: artistImage.bottom
                    horizontalCenter: artistImage.horizontalCenter
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
                text: artist === "" ? qsTr("No Artist Tag") : artist
            }
        }



        //        OpacityRampEffect {
        //            sourceItem: mainRow
        //            slope: 3
        //            offset: 0.65
        //        }
        onClicked: {
            artistGridView.currentIndex = index
            artistClicked(artist)
            pageStack.push(Qt.resolvedUrl("../pages/database/AlbumListPage.qml"),{artistname:artistname});
        }
        //        function playArtistRemorse() {
        //            remorseAction(qsTr("playing artist"), function () {
        //                playArtist(artist)
        //            }, 3000)
        //        }
        //        function addArtistRemorse() {
        //            remorseAction(qsTr("adding artist"), function () {
        //                addArtist(artist)
        //            }, 3000)
        //        }
        //        Component {
        //            id: contextMenu
        //            ContextMenu {
        //                MenuItem {
        //                    text: qsTr("play artist")
        //                    onClicked: {
        //                        if (artist !== "") {
        //                            playArtistRemorse()
        //                        }
        //                    }
        //                }

        //                MenuItem {
        //                    text: qsTr("add artist to list")
        //                    onClicked: {
        //                        if (artist !== "") {
        //                            addArtistRemorse()
        //                        }
        //                    }
        //                }
        //            }
        //        }
    }
}
