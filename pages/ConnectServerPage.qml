import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: connectPage
    property alias listmodel: connectListView.model;
    SilicaListView {
            id : connectListView
            anchors.fill: parent
            anchors.margins: Theme.paddingLarge
            contentWidth: width
            header: PageHeader {
                title: qsTr("servers");
            }
            delegate: BackgroundItem {
                Label {
                    clip: true
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                        leftMargin: listPadding
                        rightMargin: listPadding
                    }
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
