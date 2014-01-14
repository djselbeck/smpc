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
        color: Theme.rgba(Theme.highlightColor, 0.1)
        Image {
            id: albumImage
            anchors.fill: parent
            sourceSize.width: width
            sourceSize.height: height
            source: coverURL
            cache: false
            asynchronous: true
            fillMode: Image.PreserveAspectCrop
            //                onWidthChanged: {
            //                    console.debug("new W: " + width)
            //                }
            //                onHeightChanged: {
            //                    console.debug("new H: " + height)
            //                }
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
//                PropertyAnimation {
//                    id: scaleOutW
//                    target: albumShowDelegate
//                    running: rotateOut.running
//                    property: "width"
//                    from: showView.height
//                    to: showView.itemWidth
//                    duration: animationDuration
//                    easing.type: Easing.InQuad
//                }
//                PropertyAnimation {
//                    id: scaleOutH
//                    target: albumShowDelegate
//                    running: rotateOut.running
//                    property: "width"
//                    from: showView.height
//                    to: showView.itemHeight
//                    duration: animationDuration
//                    easing.type: Easing.InQuad
//                }
                PropertyAnimation {
                    id: blendcolumnOut
                    target: delegateButtons
                    property: "opacity"
                    running: rotateOut.running
                    from: 1.0
                    to: 0.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {

                    }
                }
                PropertyAnimation {
                    id: blendBackgroundOut
                    target: delegateBackside
                    property: "opacity"
                    running: rotateOut.running
                    from: 1.0
                    to: 0.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {

                    }
                }
                PropertyAnimation {
                    id: blendlistOut
                    target: albumTracksListView
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
//                PropertyAnimation {
//                    id: scaleInW
//                    target: albumShowDelegate
//                    running: rotateIn.running
//                    property: "width"
//                    from: showView.itemWidth
//                    to: showView.height
//                    duration: animationDuration
//                    easing.type: Easing.InQuad
//                }
//                PropertyAnimation {
//                    id: scaleInH
//                    target: albumShowDelegate
//                    running: rotateIn.running
//                    property: "width"
//                    from: showView.itemHeight
//                    to: showView.height
//                    duration: animationDuration
//                    easing.type: Easing.InQuad
//                }
                PropertyAnimation {
                    id: blendcolumnIn
                    target: delegateButtons
                    property: "opacity"
                    running: rotateIn.running
                    from: 0.0
                    to: 1.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {

                    }
                }
                PropertyAnimation {
                    id: blendBackgroundIn
                    target: delegateBackside
                    property: "opacity"
                    running: rotateIn.running
                    from: 0.0
                    to: 1.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {

                    }
                }
                PropertyAnimation {
                    id: blendlistIn
                    target: albumTracksListView
                    property: "opacity"
                    running: rotateIn.running
                    from: 0.0
                    to: 1.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {

                    }
                }

//                PropertyAnimation {
//                    id: fadeCoverOut
//                    target: frontPage
//                    running: rotateOut.running
//                    property: "opacity"
//                    from: 1.0
//                    to: 0.3
//                    duration: animationDuration
//                    easing.type: Easing.InQuad
//                    onStopped: {

//                    }
//                }

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
                                playAlbum([artistname, title])
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
                                addAlbum([artistname, title])
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
//                            menu: contextMenu
                            property int workaroundHeight: mainColumn.height
                            height: workaroundHeight
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
                                        text: (index + 1) + ". "
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
                                        text: (length === 0 ? "" : " (" + lengthformatted + ")")
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

//                            OpacityRampEffect {
//                                sourceItem: mainColumn
//                                slope: 3
//                                offset: 0.65
//                            }
                            onClicked: {
                                albumTracksListView.currentIndex = index
                                albumTrackClicked(title, album, artist, lengthformatted, uri,
                                                  year, tracknr)
                            }
//                            function playTrackRemorse() {
//                                remorseAction(qsTr("playing track"), function () {
//                                    playSong(uri)
//                                }, 3000)
//                            }
//                            function addTrackRemorse() {
//                                remorseAction(qsTr("adding track"), function () {
//                                    addSong(uri)
//                                }, 3000)
//                            }
//                            Component {
//                                id: contextMenu
//                                ContextMenu {
//                                    MenuItem {
//                                        text: qsTr("play track")
//                                        onClicked: {
//                                            playTrackRemorse()
//                                        }
//                                    }

//                                    MenuItem {
//                                        text: qsTr("add track to list")
//                                        onClicked: {
//                                            addTrackRemorse()
//                                        }
//                                    }
//                                    onHeightChanged: {
//                                        workaroundHeight = height + mainColumn.height
//                                    }
//                                }
//                            }
                        }
                    }

                Component.onCompleted: {
                    rotateIn.running = true
//                    blendcolumnIn.start()
//                    fadeCoverOut.start()
                    delegateButtons.visible = true
                    albumTracksListView.visible = true
                    showView.interactive = false
                    console.debug("Album backpage created")
                }
                Component.onDestruction: {
                    clearTrackList()
                    console.debug("Album backpage destroyed")
                }
            }
        }
        active: false
    }

    onClicked: {
        console.debug("index: " + index + " currentindex: " + showView.currentIndex)
        // Only flip front cover
        if ( coverRotation == 0 ) {
            albumClicked(artistname, title)
            if (!flipped) {
                backsideLoader.active = true
                flipped = true
            }
        }

        //            albumGridView.currentIndex = index
        //            albumClicked(artistname, title)
    }
    //        Component.onCompleted: {
    //            console.debug("Album created: " + title)
    //        }
    //        Component.onDestruction: {
    //            console.debug("Album destroyed:" + title)
    //        }

    //        function playAlbumRemorse() {
    //            remorseAction(qsTr("playing album"), function () {
    //                playAlbum([artistname, title])
    //            }, 3000)
    //        }
    //        function addAlbumRemorse() {
    //            remorseAction(qsTr("adding album"), function () {
    //                addAlbum([artistname, title])
    //            }, 3000)
    //        }
    //        Component {
    //            id: contextMenu
    //            ContextMenu {
    //                MenuItem {
    //                    text: qsTr("play album")
    //                    onClicked: {
    //                        if (title !== "") {
    //                            playAlbumRemorse()
    //                        }
    //                    }
    //                }

    //                MenuItem {
    //                    text: qsTr("add album to list")
    //                    onClicked: {
    //                        if (title !== "") {
    //                            addAlbumRemorse()
    //                        }
    //                    }
    //                }
    //            }
    //        }
}
