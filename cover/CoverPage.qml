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

    Rectangle
    {
        anchors.fill: parent
        color: Theme.highlightBackgroundColor
        gradient: Gradient {
            GradientStop { position: 0.0; color: Theme.rgba(Theme.highlightBackgroundColor,0.4) }
            GradientStop { position: 0.3; color:  Theme.rgba(Theme.highlightBackgroundColor,0.0) }

            GradientStop { position: 0.7; color: Theme.rgba(Theme.highlightBackgroundColor,0.0) }
            GradientStop { position: 1.0; color:  Theme.rgba(Theme.highlightBackgroundColor,0.4) }
        }

    }



    onStatusChanged: {
        recheckActive();
    }

//    Image{
//        visible: (stopped||(coverimg.sourceprimary==""&&coverimg.sourcesecondary==""))
//        anchors.centerIn: coverpage
//        source: "qrc:/images/smpc-big.png"
//        width: coverpage.width-(coverpage.width/3)
//        height: width
//        smooth: true
//        opacity: 0.8
//    }

    Label {
        id: textLabel
        anchors.centerIn: parent
        width: parent.width-(2*listPadding)
        height: (parent.height/3)*2
        wrapMode: "WordWrap"
        horizontalAlignment: Text.AlignHCenter
        text: ( currentsongpage.title==="" ? "SMPC" : currentsongpage.title )
    }
    
    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: playbuttoniconsourcecover//"image://theme/icon-cover-pause"
            onTriggered: play();
        }
        
        CoverAction {
            iconSource: "image://theme/icon-cover-next-song"
            onTriggered: next();
        }
        

    }
}


