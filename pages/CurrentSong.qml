import QtQuick 1.1
import Sailfish.Silica 1.0


Page {
    id: currentsong_page
    property alias title: titleText.text;
    property alias album: albumText.text;
    property alias artist: artistText.text;
    property alias length: positionSlider.maximumValue;
    //property int lengthtextcurrent:lengthTextcurrent.text;
    property alias lengthtextcomplete:lengthTextcomplete.text;
    property alias position: positionSlider.value;
    property alias bitrate: bitrateText.text;
    property alias shuffle: shuffleButton.checked;
    property alias repeat: repeatButton.checked;
    property alias nr: nrText.text;
    property alias uri: fileText.text;
    property alias audioproperties: audiopropertiesText.text;
    property alias pospressed: positionSlider.pressed;
    property alias volume: volumeSlider.value;
    property alias volumepressed: volumeSlider.pressed;
    property bool playing;
    property int fontsize:theme.fontSizeMedium;
    property int fontsizegrey:theme.fontSizeSmall;

        SilicaFlickable{
            anchors.margins: theme.paddingMedium
            anchors {top: parent.top;left:parent.left;right:parent.right;bottom:volumeSlider.top}
            contentHeight: infocolumn.height
            clip: true
            Column {
                //anchors.fill: parent
                id: infocolumn
                PageHeader {
                    title: qsTr("current song");
                }
                anchors {left:parent.left; right: parent.right;}
                Label{text: qsTr("title:");color:theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:titleText ;text: "";color:theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("album:");color:theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:albumText ;text: "";color:theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("artist:");color:theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:artistText ;text: "";color:theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("nr.:");color:theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:nrText ;text: "";color:theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("bitrate:");color:theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:bitrateText ;text: "";color:theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("properties:");color:theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:audiopropertiesText ;text: "";color:theme.primaryColor; font.pixelSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
                Label{text: qsTr("uri:");color:theme.secondaryColor; font.pixelSize: fontsizegrey}
                Label{id:fileText ;text: "";color:theme.primaryColor; font.pixelSize:fontsize;wrapMode:"WrapAnywhere" ;anchors {left:parent.left; right: parent.right;}}
                clip: true;
            }
        }

        Slider
        {
            anchors {bottom:positionSlider.top;right:parent.right;left:parent.left}
            id: volumeSlider
            stepSize: 1;
            maximumValue: 100
            minimumValue: 0
            valueText: value+"%";
            label: qsTr("volume");
            onPressedChanged: {
                if(!pressed)
                {
                    setVolume(value);
                }
            }
            onValueChanged: {
                setVolume(value);
               // valueText = value+"%";
            }
        }

        Slider
        {
            anchors {bottom:buttonRow.top;right:parent.right;left:parent.left}
            id: positionSlider
            stepSize: 1;
            label: qsTr("position");
            valueText: "";
            onPressedChanged: {
                if(!pressed)
                {
                    seek(value);
                }
            }
            onValueChanged: {
                valueText = formatLength(value);
            }
            Label{id:lengthTextcomplete ; text: "";color:theme.primaryColor; font.pixelSize:fontsizegrey;wrapMode: "WordWrap";anchors {right: parent.right; bottom:parent.bottom}}
        }
        Row
        {
            id: buttonRow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors {bottom:parent.bottom;/*right:parent.right;left:parent.left*/}
            Switch
            {
                id:shuffleButton
                icon.source: "image://theme/icon-m-shuffle"
                onClicked: {
                    setShuffle(checked);
                }
            }
            IconButton
            {
                id: prevButton
                icon.source: "image://theme/icon-m-previous-song"
                onClicked: prev();
            }
            IconButton
            {
                id: playButton
                icon.source:playbuttoniconsource
                onClicked: play();
            }
            IconButton
            {
                id: nextButton
                icon.source: "image://theme/icon-m-next-song"
                onClicked: next();
            }
            Switch
            {
                id:repeatButton
                icon.source: "image://theme/icon-m-repeat"
                onClicked: {
                    setRepeat(checked);
                }
            }

        }

}
