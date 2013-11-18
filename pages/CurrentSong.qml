import QtQuick 2.0
import QtQuick.XmlListModel 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: currentsong_page
    property alias title: titleText.text
    property alias album: albumText.text
    property alias artist: artistText.text
    property alias length: positionSlider.maximumValue
    //property int lengthtextcurrent:lengthTextcurrent.text;
    property alias lengthtextcomplete: lengthTextcomplete.text
    property alias position: positionSlider.value
    property alias bitrate: bitrateText.text
    property alias shuffle: shuffleButton.checked
    property alias repeat: repeatButton.checked
    property alias nr: nrText.text
    property alias uri: fileText.text
    property alias audioproperties: audiopropertiesText.text
    property alias pospressed: positionSlider.pressed
    property alias volume: volumeSlider.value
    property alias volumepressed: volumeSlider.pressed
    property bool playing
    property int fontsize: Theme.fontSizeMedium
    property int fontsizegrey: Theme.fontSizeSmall
    property bool detailsvisible: true
    property bool pageactive: false

    SilicaFlickable {
        id: infoFlickable
        anchors {
            right: parent.right
            left: parent.left
            bottom: volumeSlider.top
            top: parent.top
        }
        contentHeight: contentColumn.height
        clip: true
        Column {
            id: contentColumn
            PageHeader {
                id: pageHeading
                title: qsTr("current song")
            }
            width: parent.width

            ToggleImage {
                id: coverImage
                height: infoFlickable.height - (titleText.height + albumText.height
                                                + artistText.height + pageHeading.height)
                width: height
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
                sourceprimary: coverimageurl
                sourcesecondary: artistimageurl
            }

            ScrollLabel {
                id: titleText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                anchors.left: parent.left
                anchors.right: parent.right
            }
            ScrollLabel {
                id: albumText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                anchors.left: parent.left
                anchors.right: parent.right
            }
            ScrollLabel {
                id: artistText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                anchors.left: parent.left
                anchors.right: parent.right
            }

            TextSwitch {
                checked: true
                text: qsTr("details")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: detailsvisible = checked
            }

            Label {
                text: qsTr("nr.:")
                color: Theme.secondaryColor
                font.pixelSize: fontsizegrey
                visible: detailsvisible
            }
            Label {
                id: nrText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                wrapMode: "WordWrap"
                anchors.horizontalCenter: parent.horizontalCenter
                visible: detailsvisible
            }
            Label {
                text: qsTr("bitrate:")
                color: Theme.secondaryColor
                font.pixelSize: fontsizegrey
                visible: detailsvisible
            }
            Label {
                id: bitrateText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                wrapMode: "WordWrap"
                anchors.horizontalCenter: parent.horizontalCenter
                visible: detailsvisible
            }
            Label {
                text: qsTr("properties:")
                color: Theme.secondaryColor
                font.pixelSize: fontsizegrey
                visible: detailsvisible
            }
            Label {
                id: audiopropertiesText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                wrapMode: "WordWrap"
                anchors.horizontalCenter: parent.horizontalCenter
                visible: detailsvisible
            }
            Label {
                text: qsTr("uri:")
                color: Theme.secondaryColor
                font.pixelSize: fontsizegrey
                visible: detailsvisible
            }
            Label {
                id: fileText
                text: ""
                color: Theme.primaryColor
                font.pixelSize: fontsize
                wrapMode: "WrapAnywhere"
                visible: detailsvisible
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
        }
    }
    Slider {
        width: parent.width
        anchors {
            bottom: positionSlider.top
            right: parent.right
            left: parent.left
        }
        id: volumeSlider
        stepSize: 1
        maximumValue: 100
        minimumValue: 0
        valueText: value + "%"
        label: qsTr("volume")
        onPressedChanged: {
            if (!pressed) {
                setVolume(value)
            }
        }
        onValueChanged: {
            setVolume(value)
            // valueText = value+"%";
        }
    }

    Slider {
        anchors {
            bottom: buttonRow.top
            right: parent.right
            left: parent.left
        }
        width: parent.width
        id: positionSlider
        stepSize: 1
        label: qsTr("position")
        valueText: ""
        onPressedChanged: {
            if (!pressed) {
                seek(value)
            }
        }
        onValueChanged: {
            valueText = formatLength(value)
        }
        Label {
            id: lengthTextcomplete
            text: ""
            color: Theme.primaryColor
            font.pixelSize: fontsizegrey
            wrapMode: "WordWrap"
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
        }
    }
    Row {
        id: buttonRow
        //width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors {
            bottom: parent.bottom /*right:parent.right;left:parent.left*/
        }
        Switch {
            id: shuffleButton
            icon.source: "image://theme/icon-m-shuffle"
            onClicked: {
                setShuffle(checked)
            }
        }
        IconButton {
            id: prevButton
            icon.source: "image://theme/icon-m-previous-song"
            onClicked: prev()
        }
        IconButton {
            id: playButton
            icon.source: playbuttoniconsource
            onClicked: play()
        }
        IconButton {
            id: nextButton
            icon.source: "image://theme/icon-m-next-song"
            onClicked: next()
        }
        Switch {
            id: repeatButton
            icon.source: "image://theme/icon-m-repeat"
            onClicked: {
                setRepeat(checked)
            }
        }
    }

    function makeLastFMRequestURL() {
        var url = "";
        url = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo&api_key="
                + lastfmapikey + "&artist=" + artist + "&album=" + album;
        url = url.replace(/#|\|/g, "");
        console.debug("LastFM url created: " + url);
        coverfetcherXMLModel.source = url;

        coverfetcherXMLModel.reload();

        // Fetch artist image

        url = "http://ws.audioscrobbler.com/2.0/?method=artist.getinfo&api_key="
                + lastfmapikey + "&artist=" + artist;
        artistfetcherXMLModel.source = url;
        artistfetcherXMLModel.reload();
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
            console.debug("XML status changed to: " + status)
            if (status == XmlListModel.Ready) {
                if (count > 0) {
                    console.debug("Xml model ready, count: " + count)
                    var fetchindex
                    if (count >= 3) {
                        console.debug("item: " + 3)
                        console.debug(coverfetcherXMLModel.get(3).size + ":")
                        console.debug(coverfetcherXMLModel.get(3).image)
                        fetchindex = 3
                    } else {
                        console.debug("item: " + count)
                        console.debug(coverfetcherXMLModel.get(
                                          count - 1).size + ":")
                        console.debug(coverfetcherXMLModel.get(count - 1).image)
                        fetchindex = count - 1
                    }

                    console.debug("imageurl: " + coverfetcherXMLModel.get(
                                      fetchindex).image)
                    var coverurl = coverfetcherXMLModel.get(fetchindex).image
                    if (coverurl !== coverimageurl) {
                        // global
                        coverimageurl = coverfetcherXMLModel.get(
                                    fetchindex).image
                    }
                }
            }
            if (status == XmlListModel.Error) {
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
            console.debug("XML status changed to: " + status)
            if (status == XmlListModel.Ready) {
                if (count > 0) {
                    console.debug("Xml model ready, count: " + count)
                    var fetchindex
                    if (count >= 3) {
                        console.debug("item: " + 3)
                        console.debug(artistfetcherXMLModel.get(3).size + ":")
                        console.debug(artistfetcherXMLModel.get(3).image)
                        fetchindex = 3
                    } else {
                        console.debug("item: " + count)
                        console.debug(artistfetcherXMLModel.get(
                                          count - 1).size + ":")
                        console.debug(artistfetcherXMLModel.get(count - 1).image)
                        fetchindex = count - 1
                    }

                    console.debug("artist imageurl: " + artistfetcherXMLModel.get(
                                      fetchindex).image)
                    var artisturl = artistfetcherXMLModel.get(fetchindex).image
                    if (artisturl !== artistimageurl) {
                        // global
                        artistimageurl = artistfetcherXMLModel.get(
                                    fetchindex).image
                    }
                }
            }
            if (status == XmlListModel.Error) {
                console.debug(artistfetcherXMLModel.errorString())
            }
        }
    }

    onStatusChanged: {
        if ((status === PageStatus.Activating)
                || (status === PageStatus.Active)) {
            pageactive = true
        } else {
            pageactive = false
        }
    }
}
