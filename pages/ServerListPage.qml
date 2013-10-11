import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: serverListPage
    property alias listmodel: serverListView.model;
    SilicaListView {
            id : serverListView
            anchors.fill: parent
            contentWidth: width
            header: PageHeader {
                title: qsTr("servers");
            }
            PushUpMenu {
             MenuItem {
                 text: qsTr("Add server")
                 onClicked: {
                     console.log("Clicked option add server")
                     pageStack.push(Qt.resolvedUrl("ServerEditPage.qml"),{newprofile:true});
                 }
             }
            }
            PullDownMenu {
             MenuItem {
                 text: qsTr("Add server")
                 onClicked: {
                     console.log("Clicked option add server")
                     pageStack.push(Qt.resolvedUrl("ServerEditPage.qml"),{newprofile:true});
                 }
             }
            }
            delegate: BackgroundItem {
                Label {
                    anchors.centerIn: parent
                    text: name
                }
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("ServerEditPage.qml"),{hostname:hostname,port:port,name:name,password:password,index:index,autoconnect:autoconnect,newprofile:false});
                }
            }
    }

}
