import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: mainItm
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
                    shouldScroll = false;
                    animation.running = false
                    blendout.running = false
                    blendin.running = false
                    shouldScroll = true;
                    x = parent.x
                    lbl.opacity = 1.0
                    var restPixels =  ( ( lbl.contentWidth - mainItm.width ) );
                    var restChars = ((lbl.text.length) / lbl.contentWidth) * restPixels;
                    animation.duration = Math.round(restChars * 500); // Around 2 Chars per Second scroll
                    if(animation.duration < 3000 ) {
                        animation.duration = 3000;
                    }

                    animation.to = ((mainItm.x) - lbl.contentWidth) + (mainItm.x+mainItm.width);
                    animation.running = true
                } else {
                    shouldScroll = false;
                    animation.running = false
                    blendout.running = false
                    blendin.running = false
                }
            } else {
                shouldScroll = false;
                animation.running = false;
                blendout.running = false
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
            from: mainItm.x
            to: ((mainItm.x) - lbl.contentWidth) + (mainItm.x+mainItm.width)
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
}
