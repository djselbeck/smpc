import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: coverpage
    anchors.fill: parent

    Image{
        anchors.fill: parent
        source: coverimageurl;
        fillMode: Image.PreserveAspectFit
    }


    //color: theme.secondaryColor
    
//    Label {
//        id: label
//        anchors.centerIn: parent
//        opacity:1
//        z:1
//        text: "JollaMPC"
//    }
    
    CoverActionList {
        id: coverAction
        
        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: next();
        }
        
        CoverAction {
            iconSource: playbuttoniconsourcecover//"image://theme/icon-cover-pause"
            onTriggered: play();
        }
    }
}


