import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: serverEditPage
    property alias name: profilenameInputField.text;
    property alias hostname: hostnameInputField.text
    property alias password: passwordInputField.text
    property alias port: portInputField.text;
    property alias autoconnect: autoconnectSwitch.checked;
    property int index;
    property bool newprofile;
    SilicaFlickable
    {
        anchors.fill: parent
        anchors.margins: Theme.paddingLarge;
        contentHeight: settingsContent.height + 20;
        VerticalScrollDecorator {}
        PullDownMenu {
            MenuItem {
                text: qsTr("remove server profile")
                onClicked: {
                    pageStack.pop();
                    console.debug("Delete profile requested: " + index);
                    deleteProfile(index);

                }
            }
        }

        Column
        {
            id: settingsContent;
            width: parent.width
            PageHeader {
                title: qsTr("edit profile");
            }
            Label
            {
                anchors.right: parent.right
                anchors.left: parent.left
                text: qsTr("profilename:")
            }
            TextField
            {
                anchors.right: parent.right
                anchors.left: parent.left
                id: profilenameInputField
                placeholderText:  qsTr("input profilename")
                inputMethodHints: Qt.ImhNoPredictiveText
            }

            Label
            {
                anchors.right: parent.right
                anchors.left: parent.left
                text: qsTr("hostname:")
            }
            TextField
            {
                anchors.right: parent.right
                anchors.left: parent.left
                id: hostnameInputField
                placeholderText: qsTr("input hostname or ip")
                inputMethodHints: Qt.ImhNoPredictiveText
            }
            Label{
                anchors.right: parent.right
                anchors.left: parent.left
                text:qsTr("port:")
            }
            TextField
            {
                anchors.right: parent.right
                anchors.left: parent.left
                id: portInputField
                text: "6600"
                inputMethodHints: Qt.ImhNoPredictiveText
                validator: portvalidator
            }

            Label
            {
                anchors.right: parent.right
                anchors.left: parent.left
                text: qsTr("password:")
            }
            TextField
            {
                anchors.right: parent.right
                anchors.left: parent.left
                id: passwordInputField
                inputMethodHints: Qt.ImhNoPredictiveText
                echoMode: TextInput.Password
            }
            TextSwitch
            {
                anchors.right: parent.right
                anchors.left: parent.left
                id: autoconnectSwitch;
                text: qsTr("autoconnect");
            }
        }
    }

    onStatusChanged: {
        if(status === PageStatus.Deactivating )
        {
            console.debug("edit server page deactivating");
            if(newprofile) {
                console.debug("creating new profile");
                newProfile([index,name,hostname,password,port,autoconnect?1:0]);
            }
            else
                changeProfile([index,name,hostname,password,port,autoconnect?1:0]);

        }
    }

    IntValidator{
            id:portvalidator
            top: 65536
            bottom: 1
    }

}
