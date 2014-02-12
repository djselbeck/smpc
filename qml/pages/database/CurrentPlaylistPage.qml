import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page {
    id: currentPlaylistPage
    //property alias listmodel: playlistView.model
    property alias songid: playlistView.currentIndex
    allowedOrientations: Orientation.Portrait + Orientation.Landscape
    property int lastIndex
    SilicaListView {
        id: playlistView
        clip: true
        delegate: trackDelegate

        //        anchors {
        //            right:parent.right
        //            left: parent.left
        //            top: parent.top
        //            bottom: quickControlPanel.top
        //        }
        anchors {
            fill: parent
//            bottomMargin: quickControlPanel.visibleSize
        }

        model: playlistModelVar

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
                property int workaroundHeight: mainColumn.height
                height: workaroundHeight
                menu: contextMenu
                Component {
                    id: contextMenu
                    ContextMenu {
                        MenuItem {
                            visible: !playing
                            text: qsTr("play song")
                            onClicked: playPlaylistTrack(index)
                        }
                        MenuItem {
                            text: qsTr("remove song")
                            onClicked: {
                                remove()
                            }
                        }

                        MenuItem {
                            text: qsTr("show artist")
                            onClicked: {
                                artistClicked(artist)
                                pageStack.push(Qt.resolvedUrl("AlbumListPage.qml"),{artistname:artist});
                            }
                        }

                        MenuItem {
                            text: qsTr("show album")
                            onClicked: {
                                onClicked: {
                                    albumClicked("", album)
                                    pageStack.push(Qt.resolvedUrl("AlbumTracksPage.qml"),{artistname:"",albumname:album});

                                }
                            }
                        }

                        MenuItem {
                            visible: playing
                            text: qsTr("show information")
                            onClicked: pageStack.push(currentsongpage)
                        }

                        onHeightChanged: {
                            workaroundHeight = height + mainColumn.height
                        }
                    }
                }
                //                Component.onCompleted: {
                //                    console.debug("component created: " + title);
                //                }
                Column {
                    id: mainColumn
                    clip: true
                    height: (trackRow + artistLabel
                             >= Theme.itemSizeSmall ? trackRow + artistLabel : Theme.itemSizeSmall)
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
//                 Disabled until offically supported
                GlassItem {
                    anchors.fill: parent
                    color: Theme.highlightColor
                    visible: opacity != 0.0
                    scale: 0.8
                    opacity: playing ? 1.0 : 0.0
                    Behavior on opacity { PropertyAnimation {duration: 750} }

                }
                onClicked: {
                    playlistView.currentIndex = index
                    if (!playing) {
                        parseClickedPlaylist(index)
                    } else {
                        pageStack.push(currentsongpage)
                    }
                }

                function remove() {
                    remorseAction(qsTr("Deleting"), function () {
                        deletePlaylistTrack(index)
                    }, 3000)
                }
            }
        }

        section {
            delegate: Item {
                height: Theme.itemSizeMedium
                width: parent.width
                property string album : section.split('|')[1]
                property string artist : section.split('|')[0]
                Rectangle {
                    id: sectionFillRect
                    color: Theme.rgba(Theme.highlightBackgroundColor,0.2) //Qt.rgba(0.0, 0.0, 0.0, 0.2)
                    anchors {
                        fill: parent
                    }
                    gradient: Gradient {
                        GradientStop {
                        position: 0.0
                        color: Theme.rgba(Theme.highlightBackgroundColor,0.0)
                        }
                        GradientStop {
                            position: 1.0
                            //color: Theme.rgba(Theme.highlightBackgroundColor, 0.6)
                            color: Theme.rgba(Theme.highlightBackgroundColor,0.3)
                        }
                    }
                }

                Label {
                    id: sectionLabel
                    height: parent.height
                    text: album
                    anchors {
                        left: parent.left
                        leftMargin: listPadding
                        right: sectionImage.left
                        rightMargin: Theme.paddingSmall
                    }

                    font.pixelSize: Theme.fontSizeLarge
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    maximumLineCount: 1
                    elide: Text.ElideRight
                }
                Image {
                    id: sectionImage
                    height: parent.height
                    width: height
                    anchors {
                        right:parent.right
                        rightMargin: listPadding
                    }
                    cache: false
                    asynchronous: true
                    sourceSize.height: height
                    sourceSize.width: width

                    source: "image://imagedbprovider/album/" + artist + "/" + album
                    Image
                    {
                        id: dummyImage
                        anchors.fill: parent
                        visible: (parent.status !== Image.Ready)
                        source : "image://theme/icon-l-music"
                    }
                }
                OpacityRampEffect {
                    sourceItem: sectionImage
                    direction: OpacityRamp.BottomToTop
                }
            }
            property: "section"
        }
    }

    // Delete question
    Dialog {
        id: deleteQuestionDialog
        allowedOrientations: Orientation.Portrait + Orientation.Landscape
        Column {
            width: parent.width
            spacing: 10
            anchors.margins: Theme.paddingMedium
            DialogHeader {
                acceptText: qsTr("delete playlist")
            }
            Label {
                text: qsTr("really delete playlist?")
            }
        }
        onDone: {
            if (result === DialogResult.Accepted) {
                deletePlaylist()
            }
        }
    }

    Dialog {
        id: saveplaylistDialog
        allowedOrientations: bothOrientation
        Column {
            width: parent.width
            spacing: 10
            anchors.margins: Theme.paddingMedium
            DialogHeader {
                acceptText: qsTr("save playlist")
            }
            Label {
                text: qsTr("playlist name:")
            }
            TextField {
                id: playlistNameField
                width: parent.width
                placeholderText: qsTr("input playlist name")
            }
        }
        onDone: {
            if (result === DialogResult.Accepted) {
                console.debug("Saving playlist: " + playlistNameField.text)
                savePlaylist(playlistNameField.text)
            }
            playlistNameField.text = ""
            playlistNameField.focus = false
        }
        onOpened: {
            playlistNameField.focus = true
        }
        onStatusChanged: {
            if ((status === PageStatus.Activating)
                    || (status === PageStatus.Active)) {
                quickControlPanel.open = false
            } else {
                quickControlPanel.open = true
            }
        }
    }

    Dialog {
        id: urlInputDialog
        allowedOrientations: bothOrientation
        Column {
            width: parent.width
            spacing: 10
            anchors.margins: Theme.paddingMedium
            DialogHeader {
                acceptText: qsTr("add url")
            }
            Label {
                text: qsTr("enter url:")
            }
            TextField {
                id: urlInputField
                width: parent.width
                placeholderText: qsTr("input url (http://, file://, etc)")
            }
        }
        onDone: {
            if (result === DialogResult.Accepted) {
                addSong(urlInputField.text)
            }
            urlInputField.text = ""
            urlInputField.focus = false
        }
        onOpened: {
            urlInputField.focus = true
        }
        onStatusChanged: {
            if ((status === PageStatus.Activating)
                    || (status === PageStatus.Active)) {
                quickControlPanel.open = false
            } else {
                quickControlPanel.open = true
            }
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Activating) {
            playlistView.positionViewAtIndex(lastsongid, ListView.Center)
        } else if (status === PageStatus.Active) {
            pageStack.pushAttached(currentsongpage)
        }
    }

    function parseClickedPlaylist(index) {
        playPlaylistTrack(index)
    }

//    states: [
//        State {
//            name: "portrait"
//            when: orientation === Orientation.Portrait
//            PropertyChanges {
//                target: playlistView
//                anchors.bottomMargin: quickControlPanel.visibleSize
//                anchors.rightMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        },State {
//            name: "landscape"
//            when: orientation === Orientation.Landscape
//            PropertyChanges {
//                target: playlistView
//                anchors.bottomMargin: 0
//                anchors.rightMargin: quickControlPanel.visibleSize
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        }]

    onOrientationTransitionRunningChanged: {
        if ( !orientationTransitionRunning ) {
            playlistView.currentIndex = -1
            playlistView.currentIndex = lastsongid
        }
    }
}
