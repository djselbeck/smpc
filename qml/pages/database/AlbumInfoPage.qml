import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: albumInfoPage
    allowedOrientations: bothOrientation
    property string albumname
    PageHeader {
        id: header
        title: albumname
        clip:true
    }
    SilicaFlickable{
        id: textFlickable
        anchors{
            top: header.bottom
            bottom: parent.bottom
            right:  parent.right
            left: parent.left
//            bottomMargin: quickControlPanel.visibleSize
        }
        contentHeight: albumText.implicitHeight
        clip: true

        ScrollDecorator{}
        Label {
            id: albumText
            width: parent.width
            height: implicitHeight
            text: albumInfoText
            wrapMode: "WordWrap"
        }
    }
//    states: [
//        State {
//            name: "portrait"
//            when: orientation === Orientation.Portrait
//            PropertyChanges {
//                target: textFlickable
//                anchors.bottomMargin: quickControlPanel.visibleSize
//                anchors.rightMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        },State {
//            name: "landscape"
//            when: orientation === Orientation.Landscape
//            PropertyChanges {
//                target: textFlickable
//                anchors.bottomMargin: 0
//                anchors.rightMargin: quickControlPanel.visibleSize
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        }]
}
