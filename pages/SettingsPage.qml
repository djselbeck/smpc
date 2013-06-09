import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: settingsPage;
    SilicaListView {
        anchors.fill: parent
        anchors.margins: theme.paddingMedium
        contentWidth: width
        header: PageHeader {
            title: qsTr("settings");
        }
        model: mainMenuModel
        delegate: BackgroundItem {
            Label {
                anchors.centerIn: parent
                text: name
            }
            onClicked: {
                parseClickedSettings(ident);
            }
        }
    }


    ListModel {
        id: mainMenuModel
        ListElement { name: "server settings"; ident:"servers"; icon:"icons/music.svg" }
        ListElement { name: "outputs"; ident:"outputs"; icon:"icons/music.svg" }
        ListElement { name: "about"; ident:"about"; icon:"icons/music.svg" }
    }

    function parseClickedSettings(ident)
    {
        switch (ident) {
        case "about" :
            pageStack.push(Qt.resolvedUrl("AboutPage.qml"));
            break;
        case "servers" :
            //pageStack.push(Qt.resolvedUrl("ServerListPage.qml"));
            pageStack.push(serverList);
            break;
        }
    }
}
