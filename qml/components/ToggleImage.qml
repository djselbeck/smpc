import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: tglImg
    property alias sourceprimary: primaryImg.source
    property alias sourcesecondary: secondaryImg.source
    property alias fillMode: primaryImg.fillMode
    property bool active: true
    property bool ready

    state: "primaryImageActive"
    states: [
        State {
            name: "primaryImageActive"
            PropertyChanges {
                target: primaryImg
                opacity: 1.0
            }
            PropertyChanges {
                target: secondaryImg
                opacity: 0.0
            }
        },
        State {
            name: "secondaryImageActive"
            PropertyChanges {
                target: primaryImg
                opacity: 0.0
            }
            PropertyChanges {
                target: secondaryImg
                opacity: 1.0
            }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation {
                properties: "opacity"
                duration: 500
            }
        }
    ]

    Image {
        id: primaryImg
        anchors.fill: parent
        onStatusChanged: {
            if (status == Image.Ready) {
                setActiveImage();
                tglImg.ready = true;
            } else {
                if ( secondaryImg.status != Image.Ready ) {
                    ready = false;
                }
            }
        }
        cache: false
    }
    Image {
        id: secondaryImg
        fillMode: primaryImg.fillMode
        cache: false
        anchors.fill: parent
        onStatusChanged: {
            if (status == Image.Ready) {
                setActiveImage()
                tglImg.ready = true;
            } else {
                if ( primaryImg.status != Image.Ready ) {
                    ready = false;
                }
            }
        }
    }

    /*BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        size: BusyIndicatorSize.Medium
        running: (primaryImg.status === Image.Loading
                  || secondaryImg.status === Image.Loading)
    }*/

    Timer {
        id: waitTimer
        interval: 5000
        repeat: false
        onTriggered: {
            setActiveImage()
        }
    }
    onActiveChanged: {
        // Start timer again
        if(active) {
            waitTimer.start()
        }
        else {
            waitTimer.stop()
        }
    }

    function setActiveImage() {
        waitTimer.stop()
        if (active) {
            waitTimer.start()
        }
        if (state == "primaryImageActive"
                && (secondaryImg.status === Image.Ready)) {
            state = "secondaryImageActive"
        } else if (state == "secondaryImageActive"
                   && (primaryImg.status === Image.Ready)) {
            state = "primaryImageActive"
        }
    }
}
