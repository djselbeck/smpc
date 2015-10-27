import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: albumInfoPage
    allowedOrientations: Orientation.All
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
}
