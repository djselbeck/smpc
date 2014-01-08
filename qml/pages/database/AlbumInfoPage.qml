import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: albumInfoPage
    property string albumname
    PageHeader {
        id: header
        title: albumname
        clip:true
    }
    SilicaFlickable{
        width: parent.width
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
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
}
