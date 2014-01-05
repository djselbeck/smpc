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

        Column {
            width: parent.width
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
            }
            Label {
                text: qsTr("artist downloads remaining: ") + dbStatistic.getArtistQueueSize()
            }
            Label {
                text: qsTr("album downloads remaining: ") + dbStatistic.getAlbumQueueSize()
            }
            Button {
                id: downloadArtistImagesBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("download artist images")
                onClicked: {
                    bulkDownloadArtists()
                }
            }
            Button {
                id: downloadAlbumImagesBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("download album images")
                onClicked: {
                    bulkDownloadAlbums()
                }
            }
            Button {
                id: clearBlacklistBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("clear blacklisted albums")
                onClicked: {
                    cleanupBlacklisted()
                }
            }
            Button {
                id: clearArtistBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("clear artist images")
                onClicked: {
                    clearArtists()
                }
            }
            Button {
                id: clearAlbumBtn
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("clear album images")
                onClicked: {
                    clearAlbums()
                }
            }
        }
    }
}
