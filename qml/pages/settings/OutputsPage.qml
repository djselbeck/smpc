import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page
{
    id: outputsPage
    allowedOrientations: bothOrientation
    property string artistname;
    SilicaListView {
            id : outputsListView
            anchors.fill: parent
//            anchors.bottomMargin: quickControlPanel.visibleSize
            contentWidth: width
            header: PageHeader {
                title: qsTr("outputs")
            }
            clip: true
            model: outputsModel

            ScrollDecorator {}
            delegate: ListItem {
                contentHeight: mainColumn.height
                Column{
                    id: mainColumn
                    clip: true
                    anchors {
                        right: parent.right
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                        leftMargin: listPadding
                        rightMargin: listPadding
                    }
                         TextSwitch{
                             checked: outputenabled
                             text: outputname
                             onClicked: {
                                 if(outputenabled){
                                     console.debug("disabling output:" + id)
                                     disableOutput(id);
                                     outputenabled = false;
                                 }
                                 else {
                                     console.debug("enabling output:" + id)
                                     enableOutput(id);
                                     outputenabled = true;
                                 }
                             }
                        }
                    }
                OpacityRampEffect {
                    sourceItem: mainColumn
                    slope: 3
                    offset: 0.65
                }

            }
    }
//    states: [
//        State {
//            name: "portrait"
//            when: orientation === Orientation.Portrait
//            PropertyChanges {
//                target: outputsListView
//                anchors.bottomMargin: quickControlPanel.visibleSize
//                anchors.rightMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        },State {
//            name: "landscape"
//            when: orientation === Orientation.Landscape
//            PropertyChanges {
//                target: outputsListView
//                anchors.bottomMargin: 0
//                anchors.rightMargin: quickControlPanel.visibleSize
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        }]

}
