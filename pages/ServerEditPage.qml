import QtQuick 1.1
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
    Flickable
    {
        anchors.fill: parent
        anchors.margins: theme.paddingLarge;
        contentHeight: settingsContent.height + 20;
        VerticalScrollDecorator {}
        //{fill: parent; margins: theme.paddingLarge;}
        Column
        {
            id: settingsContent;
            width: parent.width
            Heading {
                anchors.right: parent.right
                anchors.left: parent.left
                text: qsTr("edit server");
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
        if(status == PageStatus.Deactivating )
        {
            console.debug("edit server page deactivating");
            newProfile([index,name,hostname,password,port,autoconnect?1:0]);

        }
    }

    IntValidator{
            id:portvalidator
            top: 65536
            bottom: 1
    }

}
