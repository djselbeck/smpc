import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1

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
    property alias shuffle: shufflebtn.checked;
    property alias repeat: repeatbtn.checked;
    property alias nr: nrText.text;
    property alias uri: fileText.text;
    property alias audioproperties: audiopropertiesText.text;
    property alias pospressed: positionSlider.pressed;
    property bool playing;
    property int fontsize:8;
    property int fontsizegrey:7;


    Flickable{
        anchors {left:parent.left; right: parent.right;bottom:positionSlider.top;top: parent.top}
        contentHeight: infocolumn.height
        //contentWidth: infocolumn.width
        clip: true
        Column {
            id: infocolumn
            //anchors {left:parent.left; right: parent.right; top:parent.top; bottom:parent.bottom}
            anchors {left:parent.left; right: parent.right;}
            Text{text: "Title:";color:"grey";font.pointSize: fontsizegrey}
            Text{id:titleText ;text: "";color:"white";font.pointSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
            Text{text: "Album:";color:"grey";font.pointSize: fontsizegrey}
            Text{id:albumText ;text: "";color:"white";font.pointSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
            Text{text: "Artist:";color:"grey";font.pointSize: fontsizegrey}
            Text{id:artistText ;text: "";color:"white";font.pointSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
            Text{text: "Nr.:";color:"grey";font.pointSize: fontsizegrey}
            Text{id:nrText ;text: "";color:"white";font.pointSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
            Text{text: "Bitrate:";color:"grey";font.pointSize: fontsizegrey}
            Text{id:bitrateText ;text: "";color:"white";font.pointSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
            Text{text: "Properties:";color:"grey";font.pointSize: fontsizegrey}
            Text{id:audiopropertiesText ;text: "";color:"white";font.pointSize:fontsize;wrapMode: "WordWrap";anchors {left:parent.left; right: parent.right;}}
            Text{text: "FileUri:";color:"grey";font.pointSize: fontsizegrey}
            Text{id:fileText ;text: "";color:"white";font.pointSize:fontsize;wrapMode:"WrapAnywhere" ;anchors {left:parent.left; right: parent.right;}}
            clip: true;
        }
    }
//    ToolBar{
//        id: controlrow
//        anchors {bottom: parent.bottom}

//        tools: ToolBarLayout {
//            ButtonRow {
//                ToolButton{ iconSource: "toolbar-mediacontrol-backwards"; onClicked: window.prev() }

//                ToolButton {
//                    iconSource: playbuttoniconsource; onClicked: window.play()
//                }
//                ToolButton{ iconSource: "toolbar-mediacontrol-forward"; onClicked: window.next() }
//                ToolButton {
//                    iconSource: "toolbar-mediacontrol-stop"
//                    onClicked: {
//                        window.stop();
//                    }
//                }

//                ToolButton{
//                    id: repeatbtn
//                    //text: "Repeat"
//                    iconSource: "icons/repeat.svg"
//                    checkable: true
//                    onClicked: {
//                        window.setRepeat(checked);
//                    }
//                }
//                ToolButton{
//                    id: shufflebtn

//                    iconSource: "icons/shuffle.svg"
//                    checkable: true
//                    onClicked: {
//                        window.setShuffle(checked);
//                    }

//                }
//            }

//        }}

    Slider
    {
        id: positionSlider
        stepSize: 1;
        orientation: Qt.Horizontal
        valueIndicatorVisible: true
        valueIndicatorText:formatLength(value)
        onPressedChanged: {
            if(!pressed)
            {
                window.seek(value);

            }
        }

        anchors {left:parent.left; right: parent.right; bottom:positionfield.top}
        Text{id:lengthTextcurrent ;text: "";color:"white";font.pointSize:7;wrapMode: "WordWrap";anchors {left:parent.left; bottom:parent.bottom}}
        Text{id:lengthTextcomplete ; text: "";color:"white";font.pointSize:7;wrapMode: "WordWrap";anchors {right: parent.right; bottom:parent.bottom}}
    }
    Rectangle
    {
        id:positionfield
        color: "black"
        height: childrenRect.height
        anchors {left:parent.left; right: parent.right; bottom:controlrow.top}
        //                Text{id:lengthTextcurrent ;text: "";color:"white";font.pointSize:7;wrapMode: "WordWrap";anchors {left:parent.left;}}
        //                Text{id:lengthTextcomplete ;text: "";color:"white";font.pointSize:7;wrapMode: "WordWrap";anchors {right: parent.right;}}
    }

    //        anchors {left:parent.left; right: parent.right; bottom: parent.bottom}

}
