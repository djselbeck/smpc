import QtQuick 2.1
import Sailfish.Silica 1.0

BackgroundItem {
    id: albumShowDelegate
    //menu: contextMenu
    width: showView.itemWidth
    height: showView.itemHeight
    z: PathView.z
    //        scale: PathView.delegateScale
    property int coverRotation: PathView.delegateRotation
    property bool flipped: false
    transform: Rotation {
        id: delegateRotator
        origin.x: albumShowDelegate.width / 2
        axis.x: 0
        axis.y: 1
        axis.z: 0
        angle: albumShowDelegate.coverRotation
    }
    Rectangle {
        id: frontPage
        anchors.fill: parent
        anchors.margins: Theme.paddingSmall
        color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
        Image {
            id: albumImage
            antialiasing: true
            anchors.fill: parent
            /*sourceSize.width: width
            sourceSize.height: height*/
            source: coverURL
            cache: false
            asynchronous: true
            fillMode: Image.PreserveAspectCrop
        }
        Rectangle {
            id: gradientRect
            visible: true //artistImage.source!=""
            antialiasing: true
            anchors {
                fill: albumImage
            }
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

    Loader {
        id: backsideLoader
        anchors {
            fill: parent
        }

        sourceComponent: Component {
            Rectangle {
                id: delegateBackside
                color: Qt.rgba(0,0,0, 0.5)
                opacity: 0.0
                anchors {
                    fill: parent
                    margins: Theme.paddingSmall
                }
                clip: true

                property int animationDuration: 750
                transform: Rotation {
                    origin.x: delegateBackside.width / 2
                    axis.x: 0
                    axis.y: 1
                    axis.z: 0
                    angle: 180
                }

                // Rotate OUT
                PropertyAnimation {
                    id: rotateOut
                    target: albumShowDelegate
                    property: "coverRotation"
                    from: 180.0
                    to: 0.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {
                        // Cleanup the mess
                        backsideLoader.active = false
                    }
                }

                PropertyAnimation {
                    id: blendcolumnOut
                    targets: [delegateButtons,delegateBackside,albumTracksListView]
                    property: "opacity"
                    running: rotateOut.running
                    from: 1.0
                    to: 0.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {

                    }
                }

                // Rotate IN
                PropertyAnimation {
                    id: rotateIn
                    target: albumShowDelegate
                    property: "coverRotation"
                    from: 0.0
                    to: 180.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {

                    }
                }
                PropertyAnimation {
                    id: blendcolumnIn
                    targets: [delegateButtons,delegateBackside,albumTracksListView]
                    property: "opacity"
                    running: rotateIn.running
                    from: 0.0
                    to: 1.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {

                    }
                }



                    Row {
                        id: delegateButtons
                        anchors.horizontalCenter: parent.horizontalCenter
                        visible: false
                        opacity: 0.0
                        anchors {
                            top: parent.top
                        }
                        IconButton {
                            id: backButton
                            icon.source: "image://theme/icon-m-back"
                            onClicked: {
                                if ( flipped ) {
                                    rotateOut.running = true
                                    flipped = false
                                    showView.interactive = true
                                }
                            }
                        }
                        IconButton {
                            id: playButton
                            icon.source: "image://theme/icon-m-play"
                            onClicked: {
                                playAlbum(["", title])
                                if ( flipped ) {
                                    rotateOut.running = true
                                    flipped = false
                                    showView.interactive = true
                                }
                            }
                        }
                        IconButton {
                            id: addButton
                            icon.source: "image://theme/icon-m-add"
                            onClicked: {
                                addAlbum([artist, title])
                                if ( flipped ) {
                                    rotateOut.running = true
                                    flipped = false
                                    showView.interactive = true
                                }
                            }
                        }
                        IconButton {
                            id: moreButton
                            icon.source: "image://theme/icon-m-other"
                            onClicked: {
                                albumClicked(artist, title)
                                pageStack.push(Qt.resolvedUrl("../pages/database/AlbumTracksPage.qml"),{artistname:artist,albumname:title});
                                if ( flipped ) {
                                    rotateOut.running = true
                                    flipped = false
                                    showView.interactive = true
                                }
                            }
                        }
                    }
                    SilicaListView
                    {
                        id: albumTracksListView
                        clip: true
                        visible: false
                        opacity: 0.0
                        anchors {
                            left:parent.left
                            right: parent.right
                            top: delegateButtons.bottom
                            bottom: parent.bottom
                        }
                        model: albumTracksModel
                        delegate: ListItem {
                            menu: contextMenu
                            contentHeight: mainColumn.height
                            Column {
                                id: mainColumn
                                clip: true
                                height: (titleRow.height + artistLabel.height)
                                anchors {
                                    right: parent.right
                                    left: parent.left
                                    verticalCenter: parent.verticalCenter
                                    leftMargin: Theme.paddingSmall
                                    rightMargin: Theme.paddingSmall
                                }

                                Row {
                                    id: titleRow
                                    Label {
                                        text: (index + 1) + ". "
                                        anchors {
                                            verticalCenter: parent.verticalCenter
                                        }
                                        font.pixelSize: Theme.fontSizeSmall
                                    }
                                    Label {
                                        clip: true
                                        wrapMode: Text.WrapAnywhere
                                        elide: Text.ElideRight
                                        text: (title === "" ? filename : title)
                                        anchors {
                                            verticalCenter: parent.verticalCenter
                                        }
                                        font.pixelSize: Theme.fontSizeSmall
                                    }
                                    Label {
                                        text: (length === 0 ? "" : " (" + lengthformated + ")")
                                        anchors {
                                            verticalCenter: parent.verticalCenter
                                        }
                                        font.pixelSize: Theme.fontSizeSmall
                                    }
                                }
                                Label {
                                    id: artistLabel
                                    text: (artist !== "" ? artist + " - " : "") + (album !== "" ? album : "")
                                    color: Theme.secondaryColor
                                    font.pixelSize: Theme.fontSizeTiny
                                }
                            }

                            onClicked: {
                                playAlbum([artist, album]);
                                playPlaylistTrack(index);
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
                            Component {
                                id: contextMenu
                                ContextMenu {
                                    anchors{
                                        right: (parent != null ) ? parent.right : undefined
                                        left: (parent != null ) ? parent.left : undefined
                                    }

                                    MenuItem {
                                        text: qsTr("play track")
                                        font.pixelSize: Theme.fontSizeSmall
                                        onClicked: {
                                            playTrackRemorse()
                                        }
                                    }

                                    MenuItem {
                                        text: qsTr("add track to list")
                                        font.pixelSize: Theme.fontSizeSmall
                                        onClicked: {
                                            addTrackRemorse()
                                        }
                                    }
                                }
                            }
                        }
                    }

                Component.onCompleted: {
                    clearTrackList()
                    rotateIn.running = true
                    delegateButtons.visible = true
                    albumTracksListView.visible = true
                    showView.interactive = false
                }
                Component.onDestruction: {
                    clearTrackList()
                }
            }
        }
        active: false
    }

    onClicked: {
        // Only flip front cover
        if ( coverRotation == 0 ) {
            albumClicked(artist, title)
            if (!flipped) {
                backsideLoader.active = true
                flipped = true
            }
        }
    }
}
