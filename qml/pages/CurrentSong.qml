import QtQuick 2.0
import QtQuick.XmlListModel 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: currentsong_page
    property string title:mTitle
    property string album:mAlbum
    property string artist:mArtist
    property alias length: positionSlider.maximumValue
    //property int lengthtextcurrent:lengthTextcurrent.text;
    property alias lengthtextcomplete: lengthTextcomplete.text
    property alias position: positionSlider.value
    property alias bitrate: bitrateText.text
    property bool shuffle
    property bool repeat
    //    property alias shuffle: shuffleButton.checked
    //    property alias repeat: repeatButton.checked
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

    Drawer {
        id: mainDrawer
        dock: Dock.Bottom
        anchors.fill: parent
        open: true

        SilicaFlickable {
            id: infoFlickable
            anchors.fill: parent
            BackgroundItem {
                anchors.fill: parent
                onClicked: {
                    if (mainDrawer.open) {
                        mainDrawer.hide()
                    } else {
                        mainDrawer.show()
                    }
                }
            }
            PullDownMenu {
                MenuItem {
                    text: qsTr("show all tracks from album")
                    visible: album === "" ? false : true
                    onClicked: {
                        albumClicked("", album)
                    }
                }
                MenuItem {
                    text: qsTr("show albums from artist")
                    visible: artist === "" ? false : true
                    onClicked: {
                        artistClicked(artist)
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
                Column {
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
                            visible: (coverImage.sourceprimary == ""
                                      && coverImage.sourcesecondary == "")
                        }
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
                //            Button {
                //                id: showArtistBtn
                //                text: qsTr("show albums from artist")
                //                anchors.horizontalCenter: parent.horizontalCenter
                //                onClicked: {
                //                    artistClicked(artist);
                //                }
                //            }
                //            Button {
                //                id: showAlbumBtn
                //                text: qsTr("show all tracks from album")
                //                anchors.horizontalCenter: parent.horizontalCenter
                //                onClicked: {
                //                    albumClicked("",album);
                //                }
                //            }
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
                anchors {
                    right: parent.right
                    left: parent.left
                }
                id: positionSlider
                stepSize: 1
                minimumValue: 0
                enabled: true
                maximumValue: length
                value: position
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

    function makeLastFMRequestURL() {
        var artistclean = artist.replace(/[|&;$%@"<>()+,]/g, "")
        var albumclean = album.replace(/[|&;$%@"<>()+,]/g, "")

        var url = ""
        url = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo&api_key="
                + lastfmapikey + "&artist=" + artistclean + "&album=" + albumclean
//        if (albumclean != "") {
//            coverfetcherXMLModel.source = url
//            coverfetcherXMLModel.reload()
//        }

//        coverimageurl = "image://imagedbprovider/album/" + artist + "/" + album;


        // Fetch artist image
        url = "http://ws.audioscrobbler.com/2.0/?method=artist.getinfo&api_key="
                + lastfmapikey + "&artist=" + artistclean
        if (artistclean != "") {
            artistfetcherXMLModel.source = url
            artistfetcherXMLModel.reload()
        }
    }

    XmlListModel {
        id: coverfetcherXMLModel
        query: "/lfm/album/image"
        XmlRole {
            name: "image"
            query: "./string()"
        }
        XmlRole {
            name: "size"
            query: "@size/string()"
        }
        onStatusChanged: {
            if (status == XmlListModel.Ready) {
                if (count > 0) {
                    var fetchindex
                    if (count >= 4) {
                        fetchindex = 4
                    } else {
                        fetchindex = count - 1
                    }

                    var coverurl = coverfetcherXMLModel.get(fetchindex).image
                    if (coverurl !== coverimageurl) {
                        console.debug("imageurl: " + coverfetcherXMLModel.get(
                                          fetchindex).image)
                        // global
                        coverimageurl = coverfetcherXMLModel.get(
                                    fetchindex).image
                    }
                }
            }
            if (status == XmlListModel.Error) {
                coverimageurl = ""
                console.debug(coverfetcherXMLModel.errorString())
            }
        }
    }

    XmlListModel {
        id: artistfetcherXMLModel
        query: "/lfm/artist/image"
        XmlRole {
            name: "image"
            query: "./string()"
        }
        XmlRole {
            name: "size"
            query: "@size/string()"
        }
        onStatusChanged: {
            if (status == XmlListModel.Ready) {
                if (count > 0) {
                    var fetchindex
                    if (count >= 4) {
                        fetchindex = 4
                    } else {
                        fetchindex = count - 1
                    }

                    var artisturl = artistfetcherXMLModel.get(fetchindex).image
                    if (artisturl !== artistimageurl) {
                        console.debug(
                                    "artist imageurl: " + artistfetcherXMLModel.get(
                                        fetchindex).image)
                        // global
                        artistimageurl = artistfetcherXMLModel.get(
                                    fetchindex).image
                    }
                }
            }
            if (status == XmlListModel.Error) {
                artistimageurl = ""
                console.debug(artistfetcherXMLModel.errorString())
            }
        }
    }

    onStatusChanged: {
        if ((status === PageStatus.Activating)
                || (status === PageStatus.Active)) {
            pageactive = true
            quickControlPanel.open = false
        } else {
            quickControlPanel.open = true
            pageactive = false
        }
    }
}
