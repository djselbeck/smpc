import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: serverListPage
    SilicaListView {
            id : serverListView
            anchors.fill: parent
            anchors.margins: theme.paddingLarge
            contentWidth: width
            header: Heading
            {
                anchors.left: parent.left;
                anchors.right: parent.right;
                text: qsTr("serverlist")
            }
            PushUpMenu {
             MenuItem {
                 text: qsTr("Add server")
                 onClicked: {
                     console.log("Clicked option add server")
                     pageStack.push(Qt.resolvedUrl("ServerEditPage.qml"));
                 }
             }
            }
            delegate: BackgroundItem {
                Label {
                    anchors.centerIn: parent
                    text: name
                }
                onClicked: {
                    parseClickedServerListItem(ident);
                }
            }
    }
}
