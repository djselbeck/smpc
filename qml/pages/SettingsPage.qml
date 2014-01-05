import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: settingsPage;
    SilicaListView {
        anchors.fill: parent
        contentWidth: width
        header: PageHeader {
            title: qsTr("settings");
        }
        model: settingsMenuModel
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
                parseClickedSettings(ident);
            }
        }
    }


    Component.onCompleted: {
        settingsMenuModel.append({"name":qsTr("server settings"), "ident":"servers"})
        settingsMenuModel.append({"name":qsTr("database settings"), "ident":"database"})
        settingsMenuModel.append({"name":qsTr("outputs"), "ident":"outputs"})
        settingsMenuModel.append({"name":qsTr("update database"), "ident":"updatedb"})
        settingsMenuModel.append({"name":qsTr("about"), "ident":"about"})
    }

    ListModel {
        id: settingsMenuModel
    }

    function parseClickedSettings(ident)
    {
        switch (ident) {
        case "about" :
            pageStack.push(Qt.resolvedUrl("AboutPage.qml"));
            break;
        case "servers" :
            pageStack.push(serverList);
            break;
        case "updatedb" :
            updateDB();
            break;
        case "outputs" :
            requestOutputs();
            break;
        case "database" :
            pageStack.push(Qt.resolvedUrl("DatabaseSettings.qml"))
            break;
        }
    }
}
