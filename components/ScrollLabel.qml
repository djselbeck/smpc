import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    property alias text: lbl.text
    property alias font: lbl.font
    property alias color: lbl.color
    height: lbl.height

    Label {
        id: lbl
        width: contentWidth
        height: contentHeight
        property bool shouldScroll:false

        function checkAnimation() {
            if (contentWidth > parent.width) {
                anchors.horizontalCenter = undefined
                if (lbl.visible) {
                    shouldScroll = true;
                    animation.running = false
                    blendin.running = false
                    x = parent.x
                    lbl.opacity = 1.0
                    animation.running = true
                } else {
                    shouldScroll = false;
                    animation.running = false
                    blendin.running = false
                }
            } else {
                shouldScroll = false;
                animation.running = false;
                blendin.running = false;
                lbl.x = parent.x
                lbl.opacity = 1.0
                anchors.horizontalCenter = parent.horizontalCenter
            }
        }

        onContentWidthChanged: {
            checkAnimation()
        }
        onVisibleChanged: {
            lbl.x = parent.x
            lbl.opacity = 1.0
            checkAnimation()
        }

        PropertyAnimation {
            id: animation
            target: lbl
            property: "x"
            from: parent.x
            to: (parent.x) - lbl.contentWidth
            duration: ((lbl.text.length) * 1000) / 1.75

            onStarted: {

            }

            onStopped: {
                lbl.opacity = 0.0
                lbl.x = parent.x
                if (lbl.visible && lbl.shouldScroll)
                    blendin.running = true
            }
        }

        PropertyAnimation {
            id: blendin
            target: lbl
            property: "opacity"
            from: 0.0
            to: 1.0
            duration: 500
            onStopped: {
                if (lbl.visible && lbl.shouldScroll)
                    animation.running = true
            }
        }
    }

    //    Rectangle {
    //        color: "#FFFFFF"
    //        opacity:0.5
    //        anchors.fill: lbl
    //    }
}
