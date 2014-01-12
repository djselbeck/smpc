import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: mainItm
    property alias text: lbl.text
    property alias font: lbl.font
    property alias color: lbl.color
    property alias scrolling: lbl.shouldScroll
    property bool active: (Qt.application.active)
    property int minScrollTime: 5000
    height: lbl.height
    clip: true

    //    OpacityRampEffect {
    //        sourceItem: mainItm
    //        slope: 3
    //        offset: lbl.shouldScroll ? 0.65 : 1.0
    //        enabled: lbl.shouldScroll
    //    }

    //    OpacityRampEffect {
    //        direction: OpacityRamp.RightToLeft
    //        sourceItem: mainItm
    //        slope: 3
    //        offset: lbl.shouldScroll ? 0.65 : 1.0
    //        enabled:  lbl.shouldScroll
    //    }
    function checkAnimation() {
        if (lbl.contentWidth > parent.width) {
            lbl.anchors.horizontalCenter = undefined
            if (lbl.visible && active) {
                lbl.shouldScroll = false
                lbl.running = false
                lbl.blendingout = false
                lbl.blendingin = false
                lbl.shouldScroll = true
                lbl.x = x
                lbl.opacity = 1.0
                var restPixels = ((lbl.contentWidth - mainItm.width))
                var restChars = ((lbl.text.length) / lbl.contentWidth) * restPixels
                lbl.duration = Math.round(
                            restChars * 500) // Around 2 Chars per Second scroll
                if (lbl.duration < minScrollTime) {
                    lbl.duration = minScrollTime
                }

                animation.to = ((mainItm.x) - lbl.contentWidth) + (mainItm.x + mainItm.width)
                animation.running = true
            } else {
                lbl.shouldScroll = false
                lbl.running = false
                lbl.blendingout = false
                lbl.blendingin = false
            }
        } else {
            lbl.shouldScroll = false
            lbl.running = false
            lbl.blendingout = false
            lbl.blendingin = false
            lbl.x = parent.x
            lbl.opacity = 1.0
            lbl.anchors.horizontalCenter = mainItm.horizontalCenter
        }
    }

    Label {
        id: lbl
        width: contentWidth
        height: contentHeight
        property bool shouldScroll: false
        property alias running: animation.running
        property alias blendingout: blendout.running
        property alias blendingin: blendin.running
        property alias duration: animation.duration

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
            from: mainItm.x
            to: ((mainItm.x) - lbl.contentWidth) + (mainItm.x + mainItm.width)
            duration: 500
            easing.type: Easing.InOutQuad

            onStopped: {
                if (lbl.visible && lbl.shouldScroll)
                    blendout.running = true
            }
        }
        PropertyAnimation {
            id: blendout
            target: lbl
            property: "opacity"
            from: 1.0
            to: 0.0
            duration: 400
            easing.type: Easing.InQuad
            onStopped: {
                if (lbl.visible && lbl.shouldScroll) {
                    lbl.x = mainItm.x
                    blendin.running = true
                }
            }
        }

        PropertyAnimation {
            id: blendin
            target: lbl
            property: "opacity"
            from: 0.0
            to: 1.0
            duration: 750
            easing.type: Easing.OutQuad
            onStopped: {
                if (lbl.visible && lbl.shouldScroll)
                    animation.running = true
            }
        }
    }
    onWidthChanged: {
        checkAnimation()
    }
    onVisibleChanged: {
        checkAnimation()
    }
    onActiveChanged: {
        checkAnimation()
    }
}
