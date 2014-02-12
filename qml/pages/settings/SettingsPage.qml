import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Page
{
    id: settingsPage;
    allowedOrientations: bothOrientation
    SilicaListView {
        id: settingsListView
        anchors.fill: parent
//        anchors.bottomMargin: quickControlPanel.visibleSize
        clip: true
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
        settingsMenuModel.append({"name":qsTr("gui settings"), "ident":"guisettings"})
        settingsMenuModel.append({"name":qsTr("outputs"), "ident":"outputs"})
        settingsMenuModel.append({"name":qsTr("update database"), "ident":"updatedb"})
        settingsMenuModel.append({"name":qsTr("about"), "ident":"about"})
        // Debug-only
        if(mDebugEnabled) {
            settingsMenuModel.append({"name":qsTr("garbage collection"), "ident":"gc"})
        }
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
            pageStack.push(Qt.resolvedUrl("ServerListPage.qml"));
            break;
        case "updatedb" :
            updateDB();
            break;
        case "outputs" :
            requestOutputs();
            pageStack.push(Qt.resolvedUrl("OutputsPage.qml"))
            break;
        case "database" :
            pageStack.push(Qt.resolvedUrl("DatabaseSettings.qml"))
            break;
        case "guisettings" :
            pageStack.push(Qt.resolvedUrl("GUISettings.qml"))
            break;
        case "gc" :
            console.debug("Calling garbage collection")
            gc();
            console.debug("Called garbage collection")
            break;
        }
    }
//    states: [
//        State {
//            name: "portrait"
//            when: orientation === Orientation.Portrait
//            PropertyChanges {
//                target: settingsListView
//                anchors.bottomMargin: quickControlPanel.visibleSize
//                anchors.rightMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        },State {
//            name: "landscape"
//            when: orientation === Orientation.Landscape
//            PropertyChanges {
//                target: settingsListView
//                anchors.bottomMargin: 0
//                anchors.rightMargin: quickControlPanel.visibleSize
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//            }
//        }]
}
