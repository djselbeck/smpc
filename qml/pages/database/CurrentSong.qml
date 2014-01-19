import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page {
    id: currentsong_page
    property string title:mTitle
    property string album:mAlbum
    property string artist:mArtist
    //property int lengthtextcurrent:lengthTextcurrent.text;
    property alias lengthtextcomplete: lengthTextcomplete.text
    property alias bitrate: bitrateText.text
    property bool shuffle
    property bool repeat
    property alias nr: nrText.text
    property alias uri: fileText.text
    property alias audioproperties: audiopropertiesText.text
    property alias pospressed: positionSlider.pressed
    property alias volumepressed: volumeSlider.pressed
    property bool playing
    property int fontsize: Theme.fontSizeMedium
    property int fontsizegrey: Theme.fontSizeSmall
    property bool detailsvisible: true
    property bool pageactive: false


    allowedOrientations: bothOrientation

    Drawer {
        id: mainDrawer
        dock: (orientation === Orientation.Portrait || orientation === Orientation.PortraitInverted) ? Dock.Bottom : Dock.Right
        anchors.fill: parent
        open: true

        SilicaFlickable {
            id: infoFlickable
            anchors.fill: parent
            BackgroundItem {
                id: drawerOpenBackgroundItem
                anchors.fill: parent
                onClicked: {
                    if ( currentsong_page.state=="landscape" && mainDrawer.open) {
                        mainDrawer.hide()
                    } else if ( currentsong_page.state=="landscape" && !mainDrawer.open )  {
                        mainDrawer.show()
//                        volumeControl.state = "slideVisible"
                        drawerCloseTimer.start()
                    }
                }
            }
            PullDownMenu {
                MenuItem {
                    text: qsTr("show all tracks from album")
                    visible: album === "" ? false : true
                    onClicked: {
                        albumClicked("", album)
                        pageStack.push(Qt.resolvedUrl("AlbumTracksPage.qml"),{artistname:"",albumname:mAlbum});

                    }
                }
                MenuItem {
                    text: qsTr("show albums from artist")
                    visible: artist === "" ? false : true
                    onClicked: {
                        artistClicked(artist)
                        pageStack.push(Qt.resolvedUrl("AlbumListPage.qml"),{artistname:mArtist});
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

                PageHeader {
                    id: pageHeading
                    title: qsTr("current song")
                }
                // Spacing hack
                Rectangle
                {
                    opacity:0.0
                    // Center landscapeimages
                    height: (currentsong_page.height-landscapeImageRow.height)/2
                    width: parent.width
                    visible: landscapeImageRow.visible
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
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                        }
                        id: coverImage
                        property int calcheight: (infoFlickable.height
                                                  - (titleText.height + albumText.height
                                                     + artistText.height + pageHeading.height))
                        height: (calcheight > (contentColumn.width - listPadding
                                               * 2) ? (contentColumn.width
                                                       - listPadding * 2) : calcheight)
                        width: height
                        fillMode: Image.PreserveAspectFit
                        sourceprimary: coverimageurl
                        sourcesecondary: artistimageurl
                        active: visible
                        Image {
                            id: fallbackImage
                            source: "qrc:/images/smpc-big.png"
                            anchors.fill: parent
                            visible: ( !coverImage.ready )
                        }
                    }
                    Item
                    {
                        id: landscapeImageRow
                        width: parent.width
                        height: albumImgLandscape.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        clip:true
                        Image
                        {
                            id: albumImgLandscape
                            source: coverimageurl
                            width : (parent.width/2)
                            height: width
                            anchors {
                                top: parent.top
                                left: parent.left
                            }
                            cache: false
                            fillMode: Image.PreserveAspectCrop
                        }
                        Image
                        {
                            id: artistImgLandscape
                            source: artistimageurl
                            width : (parent.width/2)
                            height: width
                            anchors {
                                top: parent.top
                                left: albumImgLandscape.right
                                leftMargin: Theme.paddingSmall
                            }
                            cache: false
                            fillMode: Image.PreserveAspectCrop
                        }
                        Rectangle
                        {
                            anchors.fill: parent
                            gradient: Gradient {
                                GradientStop {
                                    position: 0.5
                                    color: Qt.rgba(0.0, 0.0, 0.0, 0.0)
                                }
                                GradientStop {
                                    position: 0.7
                                    color: Qt.rgba(0.0, 0.0, 0.0, 0.3)
                                }
                                GradientStop {
                                    position: 1.0
                                    color: Qt.rgba(0.0, 0.0, 0.0, 1.0)
                                }
                            }
                        }


                        Column
                        {
                            id: landscapeTextScrollColumn
                            anchors {
                                bottom: parent.bottom
                            }
                            width: landscapeImageRow.width


                            ScrollLabel {
                                id: titleTextLC
                                text: title
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                width: parent.width
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            ScrollLabel {
                                id: albumTextLC
                                text: album
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                width: parent.width
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                            ScrollLabel {
                                id: artistTextLC
                                text: artist
                                color: Theme.primaryColor
                                font.pixelSize: fontsize
                                width: parent.width
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }
                            }
                        }


                    }
                    // Spacing hack
                    Rectangle
                    {
                        opacity:0.0
                        // Center landscapeimages
                        height: (currentsong_page.height-landscapeImageRow.height)/2
                        width: parent.width
                        visible: landscapeImageRow.visible
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
                        text: qsTr("nr.:")
                        color: Theme.secondaryColor
                        font.pixelSize: fontsizegrey
                        anchors {
                            left: parent.left
                        }
                    }
                    Label {
                        id: nrText
                        text: ""
                        color: Theme.primaryColor
                        font.pixelSize: fontsize
                        wrapMode: "WordWrap"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Label {
                        text: qsTr("bitrate:")
                        color: Theme.secondaryColor
                        font.pixelSize: fontsizegrey
                    }
                    Label {
                        id: bitrateText
                        text: ""
                        color: Theme.primaryColor
                        font.pixelSize: fontsize
                        wrapMode: "WordWrap"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Label {
                        text: qsTr("properties:")
                        color: Theme.secondaryColor
                        font.pixelSize: fontsizegrey
                    }
                    Label {
                        id: audiopropertiesText
                        text: ""
                        color: Theme.primaryColor
                        font.pixelSize: fontsize
                        wrapMode: "WordWrap"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Label {
                        text: qsTr("uri:")
                        color: Theme.secondaryColor
                        font.pixelSize: fontsizegrey
                    }
                    Label {
                        id: fileText
                        text: ""
                        color: Theme.primaryColor
                        font.pixelSize: fontsize
                        wrapMode: "WrapAnywhere"

                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                    }
                }
            }
        }



        backgroundSize: volumeControl.height + positionSlider.height + buttonRow.height
        background: Column {
            id: backgroundColumn
            anchors.fill: parent
            Item {
                id: volumeControl
                width: parent.width
                height: volumeSlider.height
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
                    icon.source: "image://theme/icon-status-volume-max"
                    onClicked: {
                        volumeControl.state = "sliderVisible"
                        volumeSliderFadeOutTimer.start()
                    }
                }

                Slider {
                    anchors.fill: parent

                    id: volumeSlider
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
                            mVolume = value;
                            value  = Qt.binding(function() {return mVolume;});
                            volumeControl.state = "sliderInvisible"
                        } else {
                            volumeChanging = true
                            volumeSliderFadeOutTimer.stop()
                        }
                    }
                    onValueChanged: {
                        if(pressed)
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
                maximumValue: ( mLength > 0 ) ? mLength : 1.0
                minimumValue: 0.0
                value: mPosition
                valueText: formatLength(value)
                label: qsTr("position")
                Label {
                    id: lengthTextcomplete
                    text: ""
                    color: Theme.primaryColor
                    font.pixelSize: fontsizegrey
                    wrapMode: "WordWrap"
                    anchors {
                        right: parent.right
                        rightMargin: Theme.paddingLarge
                        bottom: parent.bottom
                    }
                }
                onPressedChanged: {
                    if (!pressed) {
                        seek(value)
                        value  = Qt.binding(function() {return mPosition;});
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
                        checked  = Qt.binding(function() {return mShuffle;});
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
                        mRepeat = checked;
                        checked  = Qt.binding(function() {return mRepeat;});
                        setRepeat(checked)
                    }
                }
            }
        }
    }


    onStatusChanged: {
        if ((status === PageStatus.Activating)
                || (status === PageStatus.Active)) {
            pageactive = true
//            positionSlider.handleVisible = false;
//            positionSlider.handleVisible = true;
//            positionSlider.valueText = Qt.binding(function () {return formatLength(positionSlider.value);})
            quickControlPanel.open = false
        } else {
            quickControlPanel.open = true
            pageactive = false
        }
    }

    states: [
        State
        {
            name: "portrait"
            PropertyChanges {
                target: coverImage
                visible: true
            }
            PropertyChanges {
                target: titleText
                visible: true
            }
            PropertyChanges {
                target: artistText
                visible: true
            }
            PropertyChanges {
                target: albumText
                visible: true
            }
            PropertyChanges {
                target: mainDrawer
                open: true
            }
            PropertyChanges {
                target: pageHeading
                visible: true
            }
            PropertyChanges {
                target: landscapeImageRow
                visible: false
            }
            PropertyChanges {
                target: drawerCloseTimer
                running: false
            }
            PropertyChanges {
                target: drawerOpenBackgroundItem
                enabled: false
            }
            PropertyChanges {
                target: shuffleButton
                visible: true
            }
            PropertyChanges {
                target: repeatButton
                visible: true
            }
        },
        State
        {
            name: "landscape"
            PropertyChanges {
                target: coverImage
                visible: false
            }
            PropertyChanges {
                target: titleText
                visible: false
            }
            PropertyChanges {
                target: artistText
                visible: false
            }
            PropertyChanges {
                target: albumText
                visible: false
            }
            PropertyChanges {
                target: mainDrawer
                open: false
            }
            PropertyChanges {
                target: pageHeading
                visible: false
            }
            PropertyChanges {
                target: landscapeImageRow
                visible: true
            }
            PropertyChanges {
                target: drawerOpenBackgroundItem
                enabled: true
            }
            PropertyChanges {
                target: shuffleButton
                visible: false
            }
            PropertyChanges {
                target: repeatButton
                visible: false
            }
        }
    ]
    state : (orientation === Orientation.Portrait ) ? "portrait" : "landscape"

    Timer{
        id: drawerCloseTimer
        interval: 5000
        repeat: false
        onTriggered: {
            mainDrawer.hide()
        }
    }
}
