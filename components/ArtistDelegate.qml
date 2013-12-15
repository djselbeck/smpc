import QtQuick 2.0
import Sailfish.Silica 1.0

Component {
    ListItem {
        menu: contextMenu
        Column {
            id: mainColumn
            clip: true
            anchors {
                right: parent.right
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: listPadding
                rightMargin: listPadding
            }
            Label {
                text: (artist === "" ? "No Artist Tag" : artist)
            }
        }
        OpacityRampEffect {
            sourceItem: mainColumn
            slope: 3
            offset: 0.65
        }
        onClicked: {
            artistListView.currentIndex = index;
            artistClicked(artist)
        }
        function playArtistRemorse() {
            remorseAction(qsTr("playing artist"), function () {
                playArtist(artist)
            }, 3000)
        }
        function addArtistRemorse() {
            remorseAction(qsTr("adding artist"), function () {
                addArtist(artist)
            }, 3000)
        }
        Component {
            id: contextMenu
            ContextMenu {
                MenuItem {
                    text: qsTr("play artist")
                    onClicked: {
                        if (artist !== "") {
                            playArtistRemorse()
                        }
                    }
                }

                MenuItem {
                    text: qsTr("add artist to list")
                    onClicked: {
                        if (artist !== "") {
                            addArtistRemorse()
                        }
                    }
                }
            }
        }
    }
}
