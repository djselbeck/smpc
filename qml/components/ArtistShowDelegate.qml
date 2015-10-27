import QtQuick 2.1
import Sailfish.Silica 1.0

BackgroundItem {
    id: artistShowDelegate
    //menu: contextMenu
    antialiasing: true
    width: showView.itemWidth
    height: showView.itemHeight
    z: PathView.z
    //        scale: PathView.delegateScale
    property int coverRotation: PathView.delegateRotation
    property bool flipped: false
    transform: Rotation {
        id: delegateRotator
        origin.x: artistShowDelegate.width / 2
        axis.x: 0
        axis.y: 1
        axis.z: 0
        angle: artistShowDelegate.coverRotation
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
            source: imageURL
            cache: false
            asynchronous: true
            fillMode: Image.PreserveAspectCrop
        }
        Rectangle {
            id: gradientRect
            visible: true
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
            text: artist === "" ? qsTr("No Artist Tag") : artist
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
                    target: artistShowDelegate
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
                    targets: [delegateButtons,delegateBackside]
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
                    target: artistShowDelegate
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
                    targets: [delegateButtons,delegateBackside]
                    property: "opacity"
                    running: rotateIn.running
                    from: 0.0
                    to: 1.0
                    duration: animationDuration
                    easing.type: Easing.InQuad
                    onStopped: {

                    }
                }


                    Column {
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
                            height: backsideLoader.height / 4
                            width: height
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
                            height: backsideLoader.height / 4
                            width: height
                            onClicked: {
                                playArtist(artist)
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
                            height: backsideLoader.height / 4
                            width: height
                            onClicked: {
                                addArtist(artist)
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
                            height: backsideLoader.height / 4
                            width: height
                            onClicked: {
                                artistClicked(artist)
                                pageStack.push(Qt.resolvedUrl("../pages/database/AlbumListPage.qml"),{artistname:artistname});
                                if ( flipped ) {
                                    rotateOut.running = true
                                    flipped = false
                                    showView.interactive = true
                                }
                            }
                        }
                    }


                Component.onCompleted: {
                    rotateIn.running = true
                    delegateButtons.visible = true
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
            if (!flipped) {
                backsideLoader.active = true
                flipped = true
            }
        }
    }

}
