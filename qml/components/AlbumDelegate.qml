import QtQuick 2.0
import Sailfish.Silica 1.0


Component {
    id: albumDelegate
    ListItem {
        menu: contextMenu
        property int workaroundHeight: mainRow.height
        height:  workaroundHeight
        Row {
            id: mainRow
            height: Theme.itemSizeSmall
            anchors {
                right: parent.right
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: listPadding
                rightMargin: listPadding
            }
            Label {
                id: albumLabel
                anchors.verticalCenter: parent.verticalCenter
                text: (title === "" ? qsTr("no album tag") : title)
            }
        }
        OpacityRampEffect {
            sourceItem: mainRow
            slope: 3
            offset: 0.65
        }

        onClicked: {
            albumListView.currentIndex = index;
            albumClicked(artistname, title)
        }
        function playAlbumRemorse() {
            remorseAction(qsTr("playing album"), function () {
                playAlbum([artistname, title])
            }, 3000)
        }
        function addAlbumRemorse() {
            remorseAction(qsTr("adding album"), function () {
                addAlbum([artistname, title])
            }, 3000)
        }
        Component {
            id: contextMenu
            ContextMenu {
                MenuItem {
                    text: qsTr("play album")
                    onClicked: {
                        if (title !== "") {
                            playAlbumRemorse()
                        }
                    }
                }

                MenuItem {
                    text: qsTr("add album to list")
                    onClicked: {
                        if (title !== "") {
                            addAlbumRemorse()
                        }
                    }
                }
                onHeightChanged: {
                    workaroundHeight = height + mainRow.height
                }
            }
        }
    }
}
