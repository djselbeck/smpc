import QtQuick 1.1
import Sailfish.Silica 1.0

Rectangle {
    id: coverpage
    anchors.fill: parent
    opacity: 0.5
    color: theme.secondaryColor
    
    Label {
        id: label
        anchors.centerIn: parent
        opacity:1
        z:1
        text: "JollaMPC"
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


