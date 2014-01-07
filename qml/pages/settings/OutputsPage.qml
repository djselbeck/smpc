import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page
{
    id: outputsPage
    property alias listmodel: outputsListView.model;
    property string artistname;
    SilicaListView {
            id : outputsListView
            anchors.fill: parent
            contentWidth: width
            header: PageHeader {
                title: qsTr("outputs")
            }
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

}
