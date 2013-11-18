import QtQuick 2.0

Item {
    id: tglImg
    property string sourceprimary;
    property string sourcesecondary;
    property alias fillMode: mainImg.fillMode

    Image {
        id: mainImg
        anchors.fill: parent
    }

    function toggleSource() {
        if( mainImg.source == sourceprimary && sourcesecondary !="" ) {
            mainImg.source = sourcesecondary;
        }
        else if( mainImg.source == sourcesecondary && sourceprimary !="" ) {
            mainImg.source = sourceprimary;
        }
        else {
            if(sourceprimary !="" )
                mainImg.source = sourceprimary;
            else if ( sourcesecondary != "" )
                mainImg.source = sourcesecondary;
        }
    }

    PropertyAnimation {
        id: blendout
        target: mainImg
        property: "opacity"
        from: 1.0
        to: 0.0
        duration: 400
        easing.type: Easing.InQuad
        onStopped: {
            toggleSource();
            blendin.start();
        }
    }

    PropertyAnimation {
        id: blendin
        target: mainImg
        property: "opacity"
        from: 0.0
        to: 1.0
        duration: 750
        easing.type: Easing.OutQuad
        onStopped: {
            waitTimer.start();
        }
    }

    Timer {
        id: waitTimer
        interval: 5000
        repeat: false
        onTriggered: {
            blendout.start();
        }
    }
    onVisibleChanged: {
        if (visible) {
            console.debug("Setting primary source, visiblity changed")
            mainImg.opacity = 1.0;
            mainImg.source = sourceprimary
            waitTimer.start();
        }
        else {
            waitTimer.stop();
            blendin.stop();
        }
    }
}
