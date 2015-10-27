import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page {
    id: currentPlaylistPage
    //property alias listmodel: playlistView.model
    allowedOrientations: Orientation.All
    property int lastIndex: lastsongid
    property bool mDeleteRemorseRunning: false

    Component.onDestruction: {
        mPlaylistPage = null
    }
    Row {
        anchors.fill: parent
        SilicaListView {
            id: playlistView
            clip: true
            delegate: trackDelegate
            currentIndex: lastsongid

            anchors {
                top: parent.top
                bottom: parent.bottom
            }

            width: parent.width / 2

            model: playlistModelVar
            quickScrollEnabled: jollaQuickscroll
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 0
            header: PageHeader {
                title: qsTr("playlist")
            }
            //        populate: Transition {
            //            NumberAnimation { properties: "x"; from:playlistView.width*2 ;duration: populateDuration }
            //        }
            PullDownMenu {
                MenuItem {
                    text: qsTr("add url")
                    onClicked: {
                        pageStack.push(urlInputDialog)
                    }
                }
                MenuItem {
                    text: qsTr("delete playlist")
                    onClicked: {
                        pageStack.push(deleteQuestionDialog)
                    }
                }
                MenuItem {
                    text: qsTr("save playlist")
                    onClicked: {
                        pageStack.push(saveplaylistDialog)
                    }
                }
                MenuItem {
                    text: qsTr("open playlist")
                    onClicked: {
                        requestSavedPlaylists()
                        pageStack.push(Qt.resolvedUrl("SavedPlaylistsPage.qml"))
                    }
                }
                MenuItem {
                    text: qsTr("jump to playing song")
                    onClicked: {
                        playlistView.currentIndex = -1
                        playlistView.currentIndex = lastsongid
                    }
                }
            }

            SpeedScroller {
                listview: playlistView
            }
            ScrollDecorator {
            }
            Component {
                id: trackDelegate
                ListItem {
                    contentHeight: mainColumn.height
                    menu: contextMenu
                    Component {
                        id: contextMenu
                        ContextMenu {
                            //                        MenuItem {
                            //                            visible: !playing
                            //                            text: qsTr("play song")
                            //                            onClicked: playPlaylistTrack(index)
                            //                        }
                            MenuItem {
                                text: qsTr("remove song")
                                visible: !mDeleteRemorseRunning
                                enabled: !mDeleteRemorseRunning
                                onClicked: {
                                    mDeleteRemorseRunning = true
                                    remove()
                                }
                            }

                            MenuItem {
                                text: qsTr("show artist")
                                onClicked: {
                                    artistClicked(artist)
                                    pageStack.push(Qt.resolvedUrl(
                                                       "AlbumListPage.qml"), {
                                                       artistname: artist
                                                   })
                                }
                            }

                            MenuItem {
                                text: qsTr("show album")
                                onClicked: {
                                    onClicked: {
                                        albumClicked("", album)
                                        pageStack.push(
                                                    Qt.resolvedUrl(
                                                        "AlbumTracksPage.qml"),
                                                    {
                                                        artistname: "",
                                                        albumname: album
                                                    })
                                    }
                                }
                            }
                            MenuItem {
                                visible: !playing
                                text: qsTr("play as next")
                                onClicked: playPlaylistSongNext(index)
                            }

                            MenuItem {
                                visible: playing
                                text: qsTr("show information")
                                onClicked: pageStack.navigateForward(
                                               PageStackAction.Animated)
                            }

                            MenuItem {
                                text: qsTr("add to saved list")
                                onClicked: {
                                    requestSavedPlaylists()
                                    pageStack.push(
                                                Qt.resolvedUrl(
                                                    "AddToPlaylistDialog.qml"),
                                                {
                                                    url: path
                                                })
                                }
                            }
                        }
                    }

                    Column {
                        id: mainColumn
                        clip: true
                        height: (trackRow + artistLabel >= Theme.itemSizeSmall ? trackRow + artistLabel : Theme.itemSizeSmall)
                        anchors {
                            right: parent.right
                            left: parent.left
                            verticalCenter: parent.verticalCenter
                            leftMargin: listPadding
                            rightMargin: listPadding
                        }
                        Row {
                            id: trackRow
                            Label {
                                text: (index + 1) + ". "
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                            Label {
                                clip: true
                                wrapMode: Text.WrapAnywhere
                                elide: Text.ElideRight
                                text: (title === "" ? filename + " " : title + " ")
                                font.italic: (playing) ? true : false
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
                            text: (artist !== "" ? artist + " - " : "")
                                  + (album !== "" ? album : "")
                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                    OpacityRampEffect {
                        sourceItem: mainColumn
                        slope: 3
                        offset: 0.65
                    }
                    //                 Disabled until offically supported
                    GlassItem {
                        anchors.fill: parent
                        color: Theme.highlightColor
                        visible: opacity != 0.0
                        scale: 0.8
                        opacity: playing ? 1.0 : 0.0
                        Behavior on opacity {
                            PropertyAnimation {
                                duration: 750
                            }
                        }
                    }
                    onClicked: {
                        playlistView.currentIndex = index
                        if (!playing) {
                            parseClickedPlaylist(index)
                        } else {
                            pageStack.navigateForward(PageStackAction.Animated)
                        }
                    }

                    function remove() {
                        remorseAction(qsTr("Deleting"), function () {
                            deletePlaylistTrack(index)
                            mDeleteRemorseRunning = false
                        }, 3000)
                    }
                }
            }

            section {
                delegate: Loader {
                    active: sectionsInPlaylist && visible
                    height: sectionsInPlaylist ? Theme.itemSizeMedium : 0
                    width: parent.width
                    sourceComponent: PlaylistSectionDelegate {
                        width: undefined
                    }
                }
                property: "section"
            }
        }

        // Main row end

        // Current song pane
        Item {
            id: currentSongPane
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
            width: parent.width / 2

            Drawer {
                id: mainDrawer
                dock: Dock.Bottom
                anchors.fill: parent
                open: true

                SilicaFlickable {
                    id: infoFlickable
                    anchors.fill: parent
                    PullDownMenu {
                        MenuItem {
                            text: qsTr("show all tracks from album")
                            visible: mAlbum === "" ? false : true
                            onClicked: {
                                albumClicked("", mAlbum)
                                pageStack.push(Qt.resolvedUrl(
                                                   "AlbumTracksPage.qml"), {
                                                   artistname: "",
                                                   albumname: mAlbum
                                               })
                            }
                        }
                        MenuItem {
                            text: qsTr("show albums from artist")
                            visible: mArtist === "" ? false : true
                            onClicked: {
                                artistClicked(mArtist)
                                pageStack.push(Qt.resolvedUrl(
                                                   "AlbumListPage.qml"), {
                                                   artistname: mArtist
                                               })
                            }
                        }
                    }
                    contentHeight: contentColumn.height
                    clip: true
                    Column {
                        id: contentColumn

                        clip: true
                        anchors {
                            right: parent.right
                            left: parent.left
                        }

                        Column {
                            id: subColumn
                            anchors {
                                left: parent.left
                                right: parent.right
                                leftMargin: listPadding
                                rightMargin: listPadding
                            }

                            ToggleImage {
                                enabled: showCoverNowPlaying
                                visible: showCoverNowPlaying
                                anchors {
                                    horizontalCenter: parent.horizontalCenter
                                }
                                id: coverImage
                                property int calcheight: (infoFlickable.height
                                                          - (titleText.height + albumText.height
                                                             + artistText.height))
                                height: showCoverNowPlaying ? (calcheight > (contentColumn.width - listPadding * 2) ? (contentColumn.width - listPadding * 2) : calcheight) : 0
                                width: height
                                fillMode: Image.PreserveAspectFit
                                sourceprimary: coverimageurl
                                sourcesecondary: artistimageurl
                                active: visible
                                Rectangle {
                                    color: Theme.rgba(
                                               Theme.highlightBackgroundColor,
                                               Theme.highlightBackgroundOpacity)
                                    anchors.fill: parent
                                    visible: (!coverImage.ready
                                              && showCoverNowPlaying)
                                    Image {
                                        anchors.fill: parent
                                        source: "qrc:images/pictogram.svg"
                                        sourceSize.width: Screen.width / 2
                                        sourceSize.height: Screen.width / 2
                                    }
                                }
                            }

                            ScrollLabel {
                                id: titleText
                                text: mTitle
                                color: Theme.primaryColor
                                font.pixelSize: Theme.fontSizeMedium
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            ScrollLabel {
                                id: albumText
                                text: mAlbum
                                color: Theme.primaryColor
                                font.pixelSize: Theme.fontSizeMedium
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            ScrollLabel {
                                id: artistText
                                text: mArtist
                                color: Theme.primaryColor
                                font.pixelSize: Theme.fontSizeMedium
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }

                            Label {
                                text: qsTr("track no.:")
                                color: Theme.secondaryColor
                                font.pixelSize: Theme.fontSizeSmall
                                anchors {
                                    left: parent.left
                                }
                            }
                            Label {
                                id: nrText
                                text: mTrackNr
                                color: Theme.primaryColor
                                font.pixelSize: Theme.fontSizeMedium
                                wrapMode: "WordWrap"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Label {
                                text: qsTr("playlist no.:")
                                color: Theme.secondaryColor
                                font.pixelSize: Theme.fontSizeSmall
                                anchors {
                                    left: parent.left
                                }
                            }
                            Label {
                                id: playlistnrText
                                text: (lastsongid + 1) + " / " + mPlaylistlength
                                color: Theme.primaryColor
                                font.pixelSize: Theme.fontSizeMedium
                                wrapMode: "WordWrap"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Label {
                                text: qsTr("bitrate:")
                                color: Theme.secondaryColor
                                font.pixelSize: Theme.fontSizeSmall
                            }
                            Label {
                                id: bitrateText
                                text: mBitrate
                                color: Theme.primaryColor
                                font.pixelSize: Theme.fontSizeMedium
                                wrapMode: "WordWrap"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            Label {
                                text: qsTr("properties:")
                                color: Theme.secondaryColor
                                font.pixelSize: Theme.fontSizeSmall
                            }
                            Label {
                                id: audiopropertiesText
                                text: mAudioProperties
                                color: Theme.primaryColor
                                font.pixelSize: Theme.fontSizeMedium
                                wrapMode: "WordWrap"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            Label {
                                text: qsTr("uri:")
                                color: Theme.secondaryColor
                                font.pixelSize: Theme.fontSizeSmall
                            }
                            Label {
                                id: fileText
                                text: mUri
                                color: Theme.primaryColor
                                font.pixelSize: Theme.fontSizeMedium
                                wrapMode: "WrapAnywhere"

                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                        }
                    }
                }

               //backgroundSize: volumeControl.height + positionSlider.height + buttonRow.height
                backgroundSize: backgroundColumn.height
                background: Column {
                    id: backgroundColumn
                    width: parent.width
                    Item {
                        id: volumeControl
                        width: parent.width
                        height: volumeSlider.height + Theme.paddingMedium
                        state: "sliderInvisible"
                        states: [
                            State {
                                name: "sliderVisible"
                                PropertyChanges {
                                    target: volumeSlider
                                    enabled: true
                                    opacity: 1.0
                                }
                                PropertyChanges {
                                    target: volumeButton
                                    enabled: false
                                    opacity: 0.0
                                }
                            },
                            State {
                                name: "sliderInvisible"
                                PropertyChanges {
                                    target: volumeSlider
                                    enabled: false
                                    opacity: 0.0
                                }
                                PropertyChanges {
                                    target: volumeButton
                                    enabled: true
                                    opacity: 1.0
                                }
                            }
                        ]

                        transitions: [
                            Transition {
                                NumberAnimation {
                                    properties: "opacity"
                                    duration: 500
                                }
                            }
                        ]

                        IconButton {
                            id: volumeButton
                            anchors.centerIn: parent
                            icon.source: "image://theme/icon-m-speaker"
                            onClicked: {
                                volumeControl.state = "sliderVisible"
                                volumeSliderFadeOutTimer.start()
                            }
                            icon.onStatusChanged: {
                                if (icon.status == Image.Error) {
                                    // Try old icon name before Sailfish 2.0
                                    icon.source = "image://theme/icon-status-volume-max"
                                }
                            }
                        }

                        Slider {
                            id: volumeSlider
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            stepSize: 1
                            maximumValue: 100
                            minimumValue: 0
                            value: mVolume
                            valueText: value + "%"
                            label: qsTr("volume")
                            onPressedChanged: {
                                if (!pressed) {
                                    volumeChanging = false
                                    setVolume(value)
                                    mVolume = value
                                    value = Qt.binding(function () {
                                        return mVolume
                                    })
                                    volumeControl.state = "sliderInvisible"
                                } else {
                                    volumeChanging = true
                                    volumeSliderFadeOutTimer.stop()
                                }
                            }
                            onValueChanged: {
                                if (pressed)
                                    setVolume(value)
                                // valueText = value+"%";
                            }
                        }

                        Timer {
                            id: volumeSliderFadeOutTimer
                            interval: 3000
                            repeat: false
                            onTriggered: {
                                volumeControl.state = "sliderInvisible"
                            }
                        }
                    }

                    Slider {
                        id: positionSlider
                        width: parent.width
                        stepSize: 1.0
                        maximumValue: (mLength > 0) ? mLength : 1.0
                        minimumValue: 0.0
                        value: mPosition
                        valueText: formatLength(value)
                        label: qsTr("position")
                        Label {
                            id: lengthTextcomplete
                            text: mLengthText
                            color: Theme.primaryColor
                            font.pixelSize: Theme.fontSizeSmall
                            wrapMode: "WordWrap"
                            anchors {
                                right: parent.right
                                rightMargin: Theme.paddingLarge
                                bottom: parent.bottom
                            }
                        }
                        onPressedChanged: {
                            mPositionSliderActive = pressed
                            if (!pressed) {
                                seek(value)
                                value = Qt.binding(function () {
                                    return mPosition
                                })
                            }
                        }
                    }
                    Row {
                        id: buttonRow
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: shuffleButton.height
                        Switch {
                            id: shuffleButton
                            icon.source: "image://theme/icon-m-shuffle"
                            checked: mShuffle
                            onClicked: {
                                mShuffle = checked
                                checked = Qt.binding(function () {
                                    return mShuffle
                                })
                                setShuffle(checked)
                            }
                        }
                        IconButton {
                            id: prevButton
                            icon.source: "image://theme/icon-m-previous"
                            onClicked: prev()
                        }
                        IconButton {
                            id: stopButton
                            icon.source: "qrc:images/icon-m-stop.png"
                            onClicked: stop()
                        }
                        IconButton {
                            id: playButton
                            icon.source: playbuttoniconsource
                            onClicked: play()
                        }
                        IconButton {
                            id: nextButton
                            icon.source: "image://theme/icon-m-next"
                            onClicked: next()
                        }
                        Switch {
                            id: repeatButton
                            checked: mRepeat
                            icon.source: "image://theme/icon-m-repeat"
                            onClicked: {
                                mRepeat = checked
                                checked = Qt.binding(function () {
                                    return mRepeat
                                })
                                setRepeat(checked)
                            }
                        }
                    }
                }
            }
        }
    }

    // Delete question
    DeletePlaylistDialog {
        id: deleteQuestionDialog

    }

    SavePlaylistDialog {
        id: saveplaylistDialog
    }

    URLInputDialog {
        id: urlInputDialog
    }

    onStatusChanged: {
        if (status === PageStatus.Activating) {
            playlistView.positionViewAtIndex(lastsongid, ListView.Center)
            quickControlPanel.hideControl = true
        } else if (status === PageStatus.Active) {

            if (mCurrentSongPage == undefined) {
                var currentSongComponent = Qt.createComponent(
                            Qt.resolvedUrl("CurrentSong_large.qml"))
                mCurrentSongPage = currentSongComponent.createObject(mainWindow)
            }
            pageStack.pushAttached(mCurrentSongPage)
            quickControlPanel.hideControl = true
        } else if (status === PageStatus.Deactivating) {
            quickControlPanel.hideControl = false
        }
    }

    function parseClickedPlaylist(index) {
        playPlaylistTrack(index)
    }
    onOrientationTransitionRunningChanged: {
        if (!orientationTransitionRunning) {
            playlistView.currentIndex = -1
            playlistView.currentIndex = lastsongid
        }
    }
    onLastIndexChanged: {
        playlistView.currentIndex = -1
        playlistView.currentIndex = lastIndex
    }
}
