import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: connectPage
    property alias listmodel: connectListView.model;
    SilicaListView {
            id : connectListView
            anchors.fill: parent
            anchors.margins: theme.paddingLarge
            contentWidth: width
            header: PageHeader {
                title: qsTr("servers");
            }
            delegate: BackgroundItem {
                Label {
                    anchors.centerIn: parent
                    text: name
                }
                onClicked: {
                    console.debug("Connecting to profile:" + index);
                    connectProfile(index);
                    pageStack.pop();
                }
            }
    }

}
