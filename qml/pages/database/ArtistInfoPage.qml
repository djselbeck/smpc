import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: artistInfoPage
    allowedOrientations: bothOrientation
    property string artistname
    PageHeader {
        id: header
        title: artistname
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
        contentHeight: artistText.implicitHeight

        ScrollDecorator{}
        clip:true
        Label {
            id: artistText
            width: parent.width
            height: implicitHeight
            text: artistInfoText
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
