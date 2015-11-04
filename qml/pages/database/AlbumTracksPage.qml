import QtQuick 2.1

import Sailfish.Silica 1.0
import "../../components"

Page {
    id: albumTracksPage
    allowedOrientations: Orientation.All
    property string albumname
    property string artistname
    property int lastIndex: 0

    Loader {
        id: portraitLoader
        active: false
        anchors.fill: parent
//        anchors.bottomMargin: quickControlPanel.visibleSize
        sourceComponent: Component {

            SilicaListView {
                id: albumTracksListView
                quickScrollEnabled: jollaQuickscroll
                contentWidth: width
                model: tracksModel
                clip: true
                populate: Transition {
                    NumberAnimation {
                        properties: "x"
                        from: albumTracksListView.width * 2
                        duration: populateDuration
                    }
                }
                header: Item {
                    height: headerColumn.height
                    width: albumTracksListView.width
                    Column {
                        id: headerColumn
                        height: header.height + imageRow.height
                        width: parent.width
                        PageHeader {
                            id: header
                            title: albumname
                        }
                        Row {
                            id: imageRow
                            width: parent.width
                            height: width / 2
                            Image {
                                id: artistImage
                                width: parent.width / 2
                                height: imageRow.height
                                cache: true
                                asynchronous: true
                                sourceSize.width: width
                                sourceSize.height: height
                                source: artistname == "" ? "image://imagedbprovider/artistfromalbum/" + albumname : "image://imagedbprovider/artist/" + artistname
                                fillMode: Image.PreserveAspectCrop
                                onStatusChanged: {
                                    if (status == Image.Error
                                            && albumImage.status == Image.Error) {
                                        // Disable image and set imageRow height to 0
                                        imageRow.height = 0
                                    }
                                }
                            }
                            Image {
                                id: albumImage
                                width: parent.width / 2
                                height: imageRow.height
                                cache: true
                                asynchronous: true
                                sourceSize.width: width
                                sourceSize.height: height
                                source: artistname
                                        == "" ? "image://imagedbprovider/album/"
                                                + albumname : "image://imagedbprovider/album/"
                                                + artistname + "/" + albumname
                                fillMode: Image.PreserveAspectCrop
                                onStatusChanged: {
                                    if (status == Image.Error
                                            && artistImage.status == Image.Error) {
                                        // Disable image and set imageRow height to 0
                                        imageRow.height = 0
                                    }
                                }
                            }
                        }
                    }
                    OpacityRampEffect {
                        sourceItem: headerColumn
                        direction: OpacityRamp.TopToBottom
                    }
                }
                PullDownMenu {
                    MenuItem {
                        enabled: (artistname!=="")
                        visible: enabled
                        text: qsTr("show all tracks")
                        onClicked: {
                            albumClicked("", albumname);
                            artistname = "";
                        }
                    }
                    MenuItem {
                        text: qsTr("add album")
                        onClicked: {
                            addAlbum([artistname, albumname])
                        }
                    }
                    MenuItem {
                        text: qsTr("play album")
                        onClicked: {
                            playAlbum([artistname, albumname])
                        }
                    }
                }
                delegate: trackDelegate
            }
        }
    }

    Loader {
        id: landscapeLoader
        anchors {
            fill: parent
//            rightMargin: quickControlPanel.visibleSize
        }
        active: false
        sourceComponent: Component {
            Item {
                id: landscapeItem
                anchors.fill: parent
                Column {
                    id: pictureColumn
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                    }

                    width: parent.height / 2
                    Image {
                        id: artistImageLC
                        width: height
                        height: pictureColumn.width
                        cache: true
                        asynchronous: true
                        source: artistname
                                == "" ? "image://imagedbprovider/artistfromalbum/"
                                        + albumname : "image://imagedbprovider/artist/" + artistname
                        fillMode: Image.PreserveAspectCrop
                        onStatusChanged: {
                            if (status == Image.Error
                                    && albumImageLC.status == Image.Error) {
                                // Disable image and set imageRow height to 0
                                pictureColumn.width = 0
                            }
                        }
                    }
                    Image {
                        id: albumImageLC
                        width: height
                        height: pictureColumn.width
                        cache: true
                        asynchronous: true
                        source: artistname == "" ? "image://imagedbprovider/album/"
                                                   + albumname : "image://imagedbprovider/album/"
                                                   + artistname + "/" + albumname
                        fillMode: Image.PreserveAspectCrop
                        onStatusChanged: {
                            if (status == Image.Error
                                    && artistImageLC.status == Image.Error) {
                                // Disable image and set imageRow height to 0
                                pictureColumn.width = 0
                            }
                        }
                    }
                }
                SilicaListView {
                    id: listViewLC
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        right: parent.right
                        left: pictureColumn.right
                    }
                    header: PageHeader{
                        title: albumname
                    }
                    quickScrollEnabled: jollaQuickscroll
                    PullDownMenu {
                        MenuItem {
                            enabled: (artistname!=="")
                            visible: enabled
                            text: qsTr("show all tracks")
                            onClicked: {
                                albumClicked("", albumname);
                                artistname = "";
                            }
                        }
                        MenuItem {
                            text: qsTr("add album")
                            onClicked: {
                                addAlbum([artistname, albumname])
                            }
                        }
                        MenuItem {
                            text: qsTr("play album")
                            onClicked: {
                                playAlbum([artistname, albumname])
                            }
                        }

                    }

                    model: tracksModel
                    clip: true
                    populate: Transition {
                        NumberAnimation {
                            properties: "x"
                            from: listViewLC.width * 2
                            duration: populateDuration
                        }
                    }
                    delegate: trackDelegate
                }
                OpacityRampEffect {
                    sourceItem: pictureColumn
                    direction: OpacityRamp.LeftToRight
                }
            }
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating) {

        } else if (status === PageStatus.Activating) {
            if (!orientationTransitionRunning) {
                // Activate correct loader
                if ((orientation === Orientation.Portrait) || (orientation === Orientation.PortraitInverted)) {
                    if ( landscapeLoader.active ) {
                        landscapeLoader.active = false;
                    }
                    portraitLoader.active = true
                } else if ((orientation === Orientation.Landscape) || (orientation === Orientation.LandscapeInverted)) {
                    if ( portraitLoader.active ) {
                        portraitLoader.active = false;
                    }
                    landscapeLoader.active = true
                }
            } else {
                // Deactivate both loaders
                portraitLoader.active = false
                landscapeLoader.active = false
            }
        } else if (status === PageStatus.Active) {

            requestAlbumInfo([albumname, artistname])
            pageStack.pushAttached(Qt.resolvedUrl("AlbumInfoPage.qml"), {
                                       albumname: albumname
                                   })
        }
    }
    onOrientationTransitionRunningChanged: {
        if (!orientationTransitionRunning) {
            // Activate correct loader
            if ((orientation === Orientation.Portrait) || (orientation === Orientation.PortraitInverted)) {


                portraitLoader.active = true
            } else if ((orientation === Orientation.Landscape) || (orientation === Orientation.LandscapeInverted)) {
                landscapeLoader.active = true
            }
        } else {
            // Deactivate both loaders
            portraitLoader.active = false
            landscapeLoader.active = false
        }
    }

    Component.onDestruction: {
        clearTrackList();
    }

    Component {
        id: trackDelegate
        ListItem {
            menu: contextMenu
            contentHeight: Theme.itemSizeSmall
            Column {
                id: mainColumn
                clip: true
                height: (titleRow.height + artistLabel.height
                         >= Theme.itemSizeSmall ? titleRow.height
                                                  + artistLabel.height : Theme.itemSizeSmall)
                anchors {
                    right: parent.right
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    leftMargin: listPadding
                    rightMargin: listPadding
                }

                Row {
                    id: titleRow
                    Label {
                        text: tracknr + ". "
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    Label {
                        clip: true
                        wrapMode: Text.WrapAnywhere
                        elide: Text.ElideRight
                        text: (title === "" ? filename : title)
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    Label {
                        text: (length === 0 ? "" : " (" + lengthformated + ")")
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }
                    }
                }
                Label {
                    id: artistLabel
                    text: (artist !== "" ? artist + " - " : "") + (album !== "" ? album : "")
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            OpacityRampEffect {
                sourceItem: mainColumn
                slope: 3
                offset: 0.65
            }
            onClicked: {
                //albumTracksListView.currentIndex = index
                albumTrackClicked(title, album, artist, lengthformated, path,
                                  year, tracknr,trackmbid,artistmbid,albummbid);
            }
            function playTrackRemorse() {
                remorseAction(qsTr("playing track"), function () {
                    playSong(path)
                }, 3000)
            }
            function addTrackRemorse() {
                remorseAction(qsTr("adding track"), function () {
                    addSong(path)
                }, 3000)
            }
            function addTrackAfterCurrentRemorse() {
                remorseAction(qsTr("adding track"), function () {
                    addSongAfterCurrent(path)
                }, 3000)
            }
            Component {
                id: contextMenu
                ContextMenu {
                    anchors{
                        right: (parent != null ) ? parent.right : undefined
                        left: (parent != null ) ? parent.left : undefined
                    }
                    MenuItem {
                        text: qsTr("play track")
                        onClicked: {
                            playTrackRemorse()
                        }
                    }

                    MenuItem {
                        text: qsTr("add track to list")
                        onClicked: {
                            addTrackRemorse()
                        }
                    }
                    MenuItem {
                        text: qsTr("play after current")
                        onClicked: {
                            addTrackAfterCurrentRemorse();
                        }
                    }
                    MenuItem {
                        text: qsTr("add to saved list")
                        onClicked: {
                            requestSavedPlaylists()
                            pageStack.push(Qt.resolvedUrl("AddToPlaylistDialog.qml"),{url:path});
                        }
                    }
                }
            }
        }
    }
}
