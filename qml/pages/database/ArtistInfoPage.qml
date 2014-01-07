import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: artistInfoPage
    property string artistname
    PageHeader {
        id: header
        title: artistname
        clip:true
    }
    SilicaFlickable{

        width: parent.width
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
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


}
