import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

CoverBackground {
    id: coverpage
    anchors.fill: parent

    ToggleImage{
        id: coverimg
        anchors.fill: parent
        sourceprimary: coverimageurl;
        sourcesecondary: artistimageurl;
        fillMode: Image.PreserveAspectCrop
    }
    onStatusChanged: {
        if(status === Cover.Activating || status === Cover.Active) {
            coverimg.visible = true;
        }
        else {
            coverimg.visible = false;
        }
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


