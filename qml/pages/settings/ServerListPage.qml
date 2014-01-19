import QtQuick 2.0
import Sailfish.Silica 1.0


Page
{
    id: serverListPage
    allowedOrientations: bothOrientation
    property alias listmodel: serverListView.model;
    SilicaListView {
            id : serverListView
            anchors.fill: parent
            anchors.bottomMargin: quickControlPanel.visibleSize
            clip: true
            contentWidth: width
            header: PageHeader {
                title: qsTr("servers");
            }
            ScrollDecorator {}
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
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                        leftMargin: listPadding
                        rightMargin: listPadding
                    }
                    text: name
                }
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("ServerEditPage.qml"),{hostname:hostname,port:port,name:name,password:password,index:index,autoconnect:autoconnect,newprofile:false});
                }
            }
    }
    states: [
        State {
            name: "portrait"
            when: orientation === Orientation.Portrait
            PropertyChanges {
                target: serverListView
                anchors.bottomMargin: quickControlPanel.visibleSize
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
            }
        },State {
            name: "landscape"
            when: orientation === Orientation.Landscape
            PropertyChanges {
                target: serverListView
                anchors.bottomMargin: 0
                anchors.rightMargin: quickControlPanel.visibleSize
                anchors.leftMargin: 0
                anchors.topMargin: 0
            }
        }]

}
