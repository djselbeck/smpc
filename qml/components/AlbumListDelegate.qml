import QtQuick 2.0
import Sailfish.Silica 1.0

Component {
    id: albumDelegate
    ListItem {
        menu: contextMenu
        contentHeight: ((listImageSize  === 1) || (listImageSize  === 0)  ? Theme.itemSizeSmall : (listImageSize  === 2 ? Theme.itemSizeMedium : Theme.itemSizeLarge) )
        Row {
            id: mainRow
            height: parent.height
            anchors {
                right: parent.right
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: listPadding
                rightMargin: listPadding
            }
            Image{
                width: ( listImageSize !== 0 ) ? mainRow.height : 0
                height: mainRow.height
                sourceSize.width: width
                sourceSize.height: height
                cache: true
                asynchronous: true
                source: ( listImageSize === 0 ) ? "" : coverURL
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
            listView.currentIndex = index;
            albumClicked(artistname, title);
            pageStack.push(Qt.resolvedUrl("../pages/database/AlbumTracksPage.qml"),{artistname:artistname,albumname:title});
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
            }
        }
    }
}
