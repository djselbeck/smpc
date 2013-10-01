import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: coverpage
    anchors.fill: parent

    Image{
        anchors.fill: parent
        source: coverimageurl;
        fillMode: Image.PreserveAspectCrop
    }
    
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


