import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

CoverBackground {
    id: coverpage
    anchors.fill: parent

    function recheckActive() {
        if( (status === Cover.Activating || status === Cover.Active) ) {
            coverimg.visible = true;
        }
        else {
            coverimg.visible = false;
        }
    }

    ToggleImage{
        id: coverimg
        anchors.fill: parent
        sourceprimary: coverimageurl;
        sourcesecondary: artistimageurl;
        fillMode: Image.PreserveAspectCrop
    }

    onStatusChanged: {
        recheckActive();
    }

    Image{
        visible: (stopped||(coverimg.sourceprimary==""&&coverimg.sourcesecondary==""))
        anchors.centerIn: coverpage
        source: "qrc:/images/smpc-big.png"
        width: coverpage.width-(coverpage.width/3)
        height: width
    }
    
    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: playbuttoniconsourcecover//"image://theme/icon-cover-pause"
            onTriggered: play();
        }
        
        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: next();
        }
        

    }
}


