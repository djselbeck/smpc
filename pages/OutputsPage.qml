import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

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
            delegate: BackgroundItem {
                Column{
                    x : theme.paddingLarge
                    anchors.verticalCenter: parent.verticalCenter
                         TextSwitch{
                             checked: outputenabled
                             text: outputname
                        }
                    }
                onClicked: {
                    if(outputenabled){
                        disableOutput(id);
                        outputenabled = false;
                    }
                    else {
                        enableOutput(id);
                        outputenabled = true;
                    }
                }
            }
    }

}
