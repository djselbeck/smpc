import QtQuick 2.0

import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: songDialog
    allowedOrientations: Orientation.All
    property string title
    property string album
    property string artist
    property string lengthtext
    property string nr
    property string date
    property string filename
    property int fontsize: Theme.fontSizeMedium
    property int fontsizegrey: Theme.fontSizeSmall
    property string trackmbid
    property string albummbid
    property string artistmbid
    Loader {
        id: portraitLoader
        active: false
        anchors.fill: parent
        sourceComponent: Component {
            Item {
                anchors.fill: parent
                SilicaFlickable {
                    id: songFlickable
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: buttonRow.top
                    }

                    contentHeight: flickColumn.height
                    clip: true
                    Column {
                        id: flickColumn
                        width: parent.width
                        DialogHeader {
                            id: header
                            anchors {
                                left: parent.left
                                right: parent.right
                            }

                            acceptText: qsTr("add song")
                        }

                            Item {
                                id: imageFakeItm
                                width: parent.width
                                height: imageRow.height
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
                                    source: "image://imagedbprovider/artist/" + artist
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
                                    source: "image://imagedbprovider/album/" + artist + "/" + album
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

                                OpacityRampEffect {
                                    sourceItem: imageRow
                                    direction: OpacityRamp.TopToBottom
                                }
                        }


                        Column {
                            id: infocolumn
                            clip: true
                            anchors {
                                right: parent.right
                                left: parent.left
                                leftMargin: listPadding
                                rightMargin: listPadding
                            }

                            ScrollLabel {
                                id: titleText
                                text: title
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            ScrollLabel {
                                id: albumText
                                text: album
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            ScrollLabel {
                                id: artistText
                                text: artist
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            Label {
                                text: qsTr("length:")
                                color: Theme.secondaryColor
                                font.pixelSize: fontsizegrey
                            }
                            Label {
                                id: lengthText
                                text: lengthtext
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                wrapMode: "WordWrap"
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            Label {
                                text: qsTr("date:")
                                color: Theme.secondaryColor
                                font.pixelSize: fontsizegrey
                            }
                            Label {
                                id: dateText
                                text: date
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                wrapMode: "WordWrap"
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            Label {
                                text: qsTr("nr.:")
                                color: Theme.secondaryColor
                                font.pixelSize: fontsizegrey
                            }
                            Label {
                                id: nrText
                                text: nr
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                wrapMode: "WordWrap"
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            Label {
                                text: qsTr("Musicbrainz track id:")
                                color: Theme.secondaryColor
                                font.pixelSize: fontsizegrey
                            }
                            Label {
                                id: trackmbidText
                                text: trackmbid
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                wrapMode: "WordWrap"
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            Label {
                                text: qsTr("Musicbrainz album id:")
                                color: Theme.secondaryColor
                                font.pixelSize: fontsizegrey
                            }
                            Label {
                                id: albummbidText
                                text: albummbid
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                wrapMode: "WordWrap"
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            Label {
                                text: qsTr("Musicbrainz artist id:")
                                color: Theme.secondaryColor
                                font.pixelSize: fontsizegrey
                            }
                            Label {
                                id: artistmbidText
                                text: artistmbid
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                wrapMode: "WordWrap"
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            Label {
                                text: qsTr("uri:")
                                color: Theme.secondaryColor
                                font.pixelSize: fontsizegrey
                            }
                            Label {
                                id: fileText
                                text: filename
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                wrapMode: "WrapAnywhere"
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                        }

                        PullDownMenu {
                            MenuItem {
                                text: qsTr("add song")
                                onClicked: {
                                    accept()
                                }
                            }
                            MenuItem {
                                text: qsTr("play after current")
                                onClicked: {
                                    addSongAfterCurrent(filename)
                                    pageStack.navigateBack(
                                                PageStackAction.Animated)
                                }
                            }
                            MenuItem {
                                text: qsTr("play song")
                                onClicked: {
                                    playSong(filename)
                                    pageStack.navigateBack(
                                                PageStackAction.Animated)
                                }
                            }
                        }
                    }
                }
                Row {
                    id: buttonRow
                    height: addButton.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors {
                        bottom: parent.bottom
                    }
                    IconButton {
                        id: playButton
                        icon.source: "image://theme/icon-m-play"
                        onClicked: {
                            playSong(filename)
                            pageStack.pop()
                        }
                    }
                    IconButton {
                        id: addButton
                        icon.source: "image://theme/icon-m-add"
                        onClicked: {
                            accept()
                        }
                    }
                }
            }
        }
    }

    Loader {
        id: landscapeLoader
        active: false
        anchors.fill: parent
        sourceComponent: Component {
            Item {
                anchors.fill: parent
                Column {
                    id: imageColumn
                    anchors {
                        top: parent.top
                        left: parent.left
                        bottom: parent.bottom
                    }
                    height: parent.height
                    Image {
                        id: artistImage
                        width: height
                        height: parent.height / 2
                        cache: true
                        asynchronous: true
                        source: "image://imagedbprovider/artist/" + artist
                        fillMode: Image.PreserveAspectCrop
                        onStatusChanged: {
                            if (status == Image.Error
                                    && albumImage.status == Image.Error) {
                                // Disable image and set imageRow height to 0
                                imageColumn.width = 0
                            }
                        }
                    }
                    Image {
                        id: albumImage
                        width: height
                        height: parent.height / 2
                        cache: true
                        asynchronous: true
                        source: "image://imagedbprovider/album/" + artist + "/" + album
                        fillMode: Image.PreserveAspectCrop
                        onStatusChanged: {
                            if (status == Image.Error
                                    && artistImage.status == Image.Error) {
                                // Disable image and set imageRow height to 0
                                imageRow.width = 0
                            }
                        }
                    }
                }
                OpacityRampEffect {
                    sourceItem: imageColumn
                    direction: OpacityRamp.LeftToRight
                }
                SilicaFlickable {
                    id: songFlickable
                    anchors {
                        top: parent.top
                        left: imageColumn.right
                        right: parent.right
                        bottom: buttonRowRoot.top
                    }
                    contentHeight: infocolumn.height
                    clip: true
                    Column {
                        clip: true
                        anchors {
                            right: parent.right
                            left: parent.left
                            verticalCenter: parent.verticalCenter
                            leftMargin: listPadding
                            rightMargin: listPadding
                        }
                        id: infocolumn
                        DialogHeader {
                            id: header
                            acceptText: qsTr("add song")
                        }

                        Label {
                            id: titleText
                            text: title
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                            wrapMode: "WrapAnywhere"
                        }
                        Label {
                            id: albumText
                            text: album
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                            wrapMode: "WrapAnywhere"
                        }
                        Label {
                            id: artistText
                            text: artist
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                            wrapMode: "WrapAnywhere"
                        }
                        Label {
                            text: qsTr("length:")
                            color: Theme.secondaryColor
                            font.pixelSize: fontsizegrey
                        }
                        Label {
                            id: lengthText
                            text: lengthtext
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            wrapMode: "WordWrap"
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                        Label {
                            text: qsTr("date:")
                            color: Theme.secondaryColor
                            font.pixelSize: fontsizegrey
                        }
                        Label {
                            id: dateText
                            text: date
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            wrapMode: "WordWrap"
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                        Label {
                            text: qsTr("nr.:")
                            color: Theme.secondaryColor
                            font.pixelSize: fontsizegrey
                        }
                        Label {
                            id: nrText
                            text: nr
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            wrapMode: "WordWrap"
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                        Label {
                            text: qsTr("Musicbrainz track id:")
                            color: Theme.secondaryColor
                            font.pixelSize: fontsizegrey
                        }
                        Label {
                            id: trackmbidText
                            text: trackmbid
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            wrapMode: "WordWrap"
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                        Label {
                            text: qsTr("Musicbrainz album id:")
                            color: Theme.secondaryColor
                            font.pixelSize: fontsizegrey
                        }
                        Label {
                            id: albummbidText
                            text: albummbid
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            wrapMode: "WordWrap"
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                        Label {
                            text: qsTr("Musicbrainz artist id:")
                            color: Theme.secondaryColor
                            font.pixelSize: fontsizegrey
                        }
                        Label {
                            id: artistmbidText
                            text: artistmbid
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            wrapMode: "WordWrap"
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                        Label {
                            text: qsTr("uri:")
                            color: Theme.secondaryColor
                            font.pixelSize: fontsizegrey
                        }
                        Label {
                            id: fileText
                            text: filename
                            color: Theme.primaryColor
                            font.pixelSize: fontsize
                            wrapMode: "WrapAnywhere"
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                    }

                    PullDownMenu {
                        MenuItem {
                            text: qsTr("add song")
                            onClicked: {
                                accept()
                            }
                        }
                        MenuItem {
                            text: qsTr("play after current")
                            onClicked: {
                                addSongAfterCurrent(filename)
                                pageStack.navigateBack(PageStackAction.Animated)
                            }
                        }
                        MenuItem {
                            text: qsTr("play song")
                            onClicked: {
                                playSong(filename)
                                pageStack.navigateBack(PageStackAction.Animated)
                            }
                        }
                    }
                }
                Item {
                    id: buttonRowRoot
                    anchors {
                        left: imageColumn.right
                        bottom: parent.bottom
                        right: parent.right
                    }
                    height: buttonRow.height
                    Row {
                        id: buttonRow
                        anchors.horizontalCenter: parent.horizontalCenter
                        IconButton {
                            id: playButton
                            icon.source: "image://theme/icon-m-play"
                            onClicked: {
                                playSong(filename)
                                pageStack.pop()
                            }
                        }
                        IconButton {
                            id: addButton
                            icon.source: "image://theme/icon-m-add"
                            onClicked: {
                                accept()
                            }
                        }
                    }
                }
            }
        }
    }

    onAccepted: {
        addSong(filename)
    }
    onOrientationTransitionRunningChanged: {
        if (!orientationTransitionRunning) {
            // Activate correct loader
            if ((orientation === Orientation.Portrait)
                    || (orientation === Orientation.PortraitInverted)) {

                portraitLoader.active = true
            } else if ((orientation === Orientation.Landscape)
                       || (orientation === Orientation.LandscapeInverted)) {
                landscapeLoader.active = true
            }
        } else {
            // Deactivate both loaders
            portraitLoader.active = false
            landscapeLoader.active = false
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Activating) {
            if (!orientationTransitionRunning) {
                // Activate correct loader
                if ((orientation === Orientation.Portrait)
                        || (orientation === Orientation.PortraitInverted)) {
                    if (landscapeLoader.active) {
                        landscapeLoader.active = false
                    }
                    portraitLoader.active = true
                } else if ((orientation === Orientation.Landscape)
                           || (orientation === Orientation.LandscapeInverted)) {
                    if (portraitLoader.active) {
                        portraitLoader.active = false
                    }
                    landscapeLoader.active = true
                }
            } else {
                // Deactivate both loaders
                portraitLoader.active = false
                landscapeLoader.active = false
            }
        }
    }
}
