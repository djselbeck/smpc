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
        PullDownMenu {
            MenuItem {
                text: qsTr("show all tracks from album")
                visible: album === "" ? false : true;
                onClicked: {
                    albumClicked("",album);
                }
         }
            MenuItem {
                text: qsTr("show albums from artist")
                visible: artist === "" ? false : true;
                onClicked: {
                    artistClicked(artist);
                }
         }
        }
        contentHeight: contentColumn.height
        clip: true
        Column {
            id: contentColumn
            PageHeader {
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                id: pageHeading
                title: qsTr("current song")
            }
            clip: true
            anchors {
                right: parent.right
                left: parent.left
                leftMargin: listPadding
                rightMargin: listPadding
            }

            ToggleImage {
                id: coverImage
                height: infoFlickable.height - (titleText.height + albumText.height
                                                + artistText.height + pageHeading.height)
                width: height
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
                sourceprimary: coverimageurl
                sourcesecondary: artistimageurl
                Image{
                    id:fallbackImage
                    source: "qrc:/images/smpc-big.png"
                    anchors.fill: parent
                    visible: (coverImage.sourceprimary=="" && coverImage.sourcesecondary=="")
                }
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
        valueText: "0:00"
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
                rightMargin: Theme.paddingLarge
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

        var artistclean = artist.replace(/[|&;$%@"<>()+,]/g, "");
        var albumclean = album.replace(/[|&;$%@"<>()+,]/g, "");

        var url = "";
        url = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo&api_key="
                + lastfmapikey + "&artist=" + artistclean + "&album=" + albumclean;
        if( albumclean!="" ) {
            coverfetcherXMLModel.source = url;
            coverfetcherXMLModel.reload();
        }

        // Fetch artist image

        url = "http://ws.audioscrobbler.com/2.0/?method=artist.getinfo&api_key="
                + lastfmapikey + "&artist=" + artistclean;
        if(artistclean!="") {
            artistfetcherXMLModel.source = url;
            artistfetcherXMLModel.reload();
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
                coverimageurl = "";
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
                        console.debug("artist imageurl: " + artistfetcherXMLModel.get(
                                          fetchindex).image)
                        // global
                        artistimageurl = artistfetcherXMLModel.get(
                                    fetchindex).image
                    }
                }
            }
            if (status == XmlListModel.Error) {
                artistimageurl = "";
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
