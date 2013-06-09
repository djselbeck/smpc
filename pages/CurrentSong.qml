import QtQuick 1.1
import Sailfish.Silica 1.0


Page {
    id: currentsong_page
    property alias title: titleText.text;
    property alias album: albumText.text;
    property alias artist: artistText.text;
    property alias length: positionSlider.maximumValue;
    property alias lengthtextcurrent:lengthTextcurrent.text;
    property alias lengthtextcomplete:lengthTextcomplete.text;
    property alias position: positionSlider.value;
    property alias bitrate: bitrateText.text;
  //  property alias shuffle: shufflebtn.checked;
  //  property alias repeat: repeatbtn.checked;
    property alias nr: nrText.text;
    property alias uri: fileText.text;
    property alias audioproperties: audiopropertiesText.text;
    property alias pospressed: positionSlider.pressed;
    property bool playing;
    property int fontsize:theme.fontSizeMedium;
    property int fontsizegrey:theme.fontSizeSmall;



    SilicaFlickable{
        anchors {left:parent.left; right: parent.right;bottom:positionSlider.top;top: parent.top}
        anchors.margins: theme.paddingMedium
        contentHeight: infocolumn.height
        //contentWidth: infocolumn.width
        clip: true
        Column {
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
        id: positionSlider
        stepSize: 1;
        onPressedChanged: {
            if(!pressed)
            {
                seek(value);
            }
        }

        anchors {left:parent.left; right: parent.right; bottom:positionfield.top}
        Label{id:lengthTextcurrent ;text: "";color:theme.primaryColor; font.pixelSize:fontsizegrey;wrapMode: "WordWrap";anchors {left:parent.left; bottom:parent.bottom}}
        Label{id:lengthTextcomplete ; text: "";color:theme.primaryColor; font.pixelSize:fontsizegrey;wrapMode: "WordWrap";anchors {right: parent.right; bottom:parent.bottom}}
    }
    Rectangle
    {
        id:positionfield
        color: "black"
        height: childrenRect.height
        anchors {left:parent.left; right: parent.right; bottom:parent.bottom}
        //                Label{id:lengthTextcurrent ;text: "";color:theme.primaryColor; font.pixelSize:7;wrapMode: "WordWrap";anchors {left:parent.left;}}
        //                Label{id:lengthTextcomplete ;text: "";color:theme.primaryColor; font.pixelSize:7;wrapMode: "WordWrap";anchors {right: parent.right;}}
    }

    //        anchors {left:parent.left; right: parent.right; bottom: parent.bottom}

}
