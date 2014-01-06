import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    PageHeader {
        id: header
        title: qsTr("database")
    }
    SilicaFlickable {
        id: mainFlickable
        anchors {
            fill: parent
            topMargin: header.height
        }
        contentHeight: mainColumn.height
        clip: true

        Column {
            id: mainColumn
            width: parent.width
            anchors {
                leftMargin: listPadding
                rightMargin: listPadding
            }

            Label {
                text: qsTr("albums: ") + dbStatistic.getAlbumCount()
            }
            Label {
                text: qsTr("blacklisted albums: ") + dbStatistic.getAlbumBlacklistCount()
            }
            Label {
                text: qsTr("artists: ") + dbStatistic.getArtistCount()
            }
            Label {
                text: qsTr("images: ") + dbStatistic.getImageCount()
            }Label {
                text: qsTr("filesize: ") + dbStatistic.getDatabaseSize()/(1048576)+ " MB"
            }
            Label {
                text: qsTr("artist downloads remaining: ") + dbStatistic.getArtistQueueSize()
            }
            Label {
                text: qsTr("album downloads remaining: ") + dbStatistic.getAlbumQueueSize()
            }
            ComboBox {
                id: downloadSizeComboBox
                label: qsTr("download size:")
                currentIndex: downloadSize
                menu: ContextMenu {
                    MenuItem {
                        text: "small"
                    }
                    MenuItem {
                        text: "medium"
                    }
                    MenuItem {
                        text: "large"
                    }
                    MenuItem {
                        text: "extralarge"
                    }
                    MenuItem {
                        text: "mega"
                    }
                }

                onValueChanged: {
                    console.debug(
                                "Download size: " + currentIndex + ":" + value + " selected")
                    newDownloadSize(currentIndex)
                }
            }
            Label {
                id: warningLabel
                width: parent.width
                color: "red"
                text: qsTr("Although the setting \"mega\" will look the best, it will require huge amount of local data cached.")
                wrapMode: "WordWrap"
            }

            Button {
                id: downloadArtistImagesBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("download artist images")
                onClicked: {
                    bulkDownloadArtists()
                }
                enabled: dbStatistic.getArtistQueueSize() === 0
            }
            Button {
                id: downloadAlbumImagesBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("download album images")
                onClicked: {
                    bulkDownloadAlbums()
                }
                enabled: dbStatistic.getAlbumQueueSize() === 0
            }
            Button {
                id: clearBlacklistBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("clear blacklisted albums")
                onClicked: {
                    pageStack.push(dialogComponent, {
                                       confirmationRole: 0
                                   })
                }
            }
            Button {
                id: clearArtistBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("clear artist images")
                onClicked: {
                    pageStack.push(dialogComponent, {
                                       confirmationRole: 1
                                   })
                }
            }
            Button {
                id: clearAlbumBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("clear album images")
                onClicked: {
                    pageStack.push(dialogComponent, {
                                       confirmationRole: 2
                                   })
                }
            }
            Button {
                id: clearDBBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("clear complete database")
                onClicked: {
                    pageStack.push(dialogComponent, {
                                       confirmationRole: 3
                                   })
                }
            }
        }
    }

    Component {
        id: dialogComponent
        Dialog {
            id: confirmationDialog
            property int confirmationRole
            property string headerText
            property string questionText
            DialogHeader {
                id: confirmationHeader
                acceptText: qsTr("clear")
                title: headerText
            }
            Label {
                id: questionLabel
                text: questionText
                width: parent.width-(2*listPadding)
                wrapMode: Text.WordWrap
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: confirmationHeader.bottom
                }
            }

            Component.onCompleted: {
                console.debug("Created:" + confirmationRole);
                switch (confirmationRole) {
                    // Clear blacklisted albums
                case 0:
                    confirmationDialog.headerText = qsTr(
                                "clear blacklist albums")
                    confirmationDialog.questionText = qsTr(
                                "Do you really want to delete all albums which are blacklisted from local database cache? There is no turning back!")
                    break
                    // Clear artists
                case 1:
                    confirmationDialog.headerText = qsTr("clear artists")
                    confirmationDialog.questionText = qsTr(
                                "Do you really want to delete all artists from local database cache? There is no turning back!")
                    break
                    // Clear albums
                case 2:
                    confirmationDialog.headerText = qsTr("clear albums")
                    confirmationDialog.questionText = qsTr(
                                "Do you really want to delete all albums from local database cache? There is no turning back!")
                    break
                    // Clear all
                case 3:
                    confirmationDialog.headerText = qsTr("clear database")
                    confirmationDialog.questionText = qsTr(
                                "Do you really want to delete the complete local database cache? There is no turning back!")
                    break
                }
            }
            onAccepted: {
                switch (confirmationRole) {
                case 0:
                    cleanupBlacklisted()
                    break
                case 1:
                    cleanupArtists()
                    break
                case 2:
                    cleanupAlbums()
                    break
                case 3:
                    cleanupDB()
                    break
                }
            }
        }
    }
}
