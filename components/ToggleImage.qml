import QtQuick 2.0

Item {
    id: tglImg
    property alias sourceprimary: primaryImg.source
    property alias sourcesecondary: secondaryImg.source
    property alias fillMode: primaryImg.fillMode
    property bool primaryActive: false

    Image {
        id: primaryImg
        anchors.fill: parent
        opacity: 0.0
    }
    Image {
        id: secondaryImg
        fillMode: primaryImg.fillMode
        anchors.fill: parent
        opacity:0.0
    }


    PropertyAnimation {
        id: blendoutprimary
        target: primaryImg
        property: "opacity"
        from: 1.0
        to: 0.0
        duration: 400
        easing.type: Easing.Linear
        onStopped: {
            if(tglImg.visible) {
                //blendinsecondary.start();
            }
        }
    }

    PropertyAnimation {
        id: blendinprimary
        target: primaryImg
        property: "opacity"
        from: 0.0
        to: 1.0
        duration: 750
        easing.type: Easing.OutQuad
        onStopped: {
            if(tglImg.visible) {
                primaryActive = true;
                waitTimer.start();
            }
        }
    }

    PropertyAnimation {
        id: blendoutsecondary
        target: secondaryImg
        property: "opacity"
        from: 1.0
        to: 0.0
        duration: 400
        easing.type: Easing.Linear
        onStopped: {
            if(tglImg.visible) {
                //blendinprimary.start();
            }
        }
    }

    PropertyAnimation {
        id: blendinsecondary
        target: secondaryImg
        property: "opacity"
        from: 0.0
        to: 1.0
        duration: 750
        easing.type: Easing.OutQuad
        onStopped: {
            if(tglImg.visible) {
                primaryActive = false;
                waitTimer.start();
            }
        }
    }

    Timer {
        id: waitTimer
        interval: 5000
        repeat: false
        onTriggered: {
            if(tglImg.visible) {
                if ( primaryActive && sourcesecondary != "" ) {
                    blendoutprimary.start();
                    blendinsecondary.start();
                }
                else if ( !primaryActive && sourceprimary != "" ) {
                    blendoutsecondary.start();
                    blendinprimary.start();
                }
            }
        }
    }
    onVisibleChanged: {
        if (visible) {
            if(sourceprimary!="") {
                secondaryImg.opacity = 0.0;
                primaryImg.opacity = 1.0;
                primaryActive = true;
            } else {
                primaryImg.opacity = 0.0;
                secondaryImg.opacity = 1.0;
                primaryActive = false;
            }
            waitTimer.start();
        }
        else {
            console.debug("Disabling all");
            waitTimer.stop();
        }
    }
}
