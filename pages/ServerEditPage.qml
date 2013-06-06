import QtQuick 1.1
import Sailfish.Silica 1.0
import "../components"

Page
{
    id: serverEditPage
    property alias hostname: hostnameInputField.text
    property alias password: passwordInputField.text
    Column
    {
        anchors {fill: parent; margins: theme.paddingLarge;}
        Heading {
            anchors.right: parent.right
            anchors.left: parent.left
            text: qsTr("edit server");
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
            text: qsTr("input hostname or ip")
            inputMethodHints: Qt.ImhNoPredictiveText
            onActiveFocusChanged: {
                if(focus) {
                    console.debug("hostname input got focus");
                    text = "";
                }
                else{
                    if(text=="")
                    {
                        text = qsTr("input hostname or ip");
                    }
                }
            }
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
    }

    onStatusChanged: {
        if(status == PageStatus.Deactivating )
        {
            console.debug("edit server page deactivating");
        }
    }

    IntValidator{
            id:portvalidator
            top: 65536
            bottom: 1
    }
}
