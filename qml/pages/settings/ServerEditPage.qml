import QtQuick 2.0
import Sailfish.Silica 1.0
import "../../components"

Dialog
{
    id: serverEditPage
    allowedOrientations: Orientation.All
    property alias name: profilenameInputField.text
    property alias hostname: hostnameInputField.text
    property alias password: passwordInputField.text
    property alias port: portInputField.text
    property alias streamingport: streamingPortInputField.text
    property alias autoconnect: autoconnectSwitch.checked
    property alias macaddress: macAddressField.text
    property int index
    property bool newprofile
    SilicaFlickable
    {
        id: serverSettingFlickable
        anchors.fill: parent
        clip: true
        contentHeight: settingsContent.height
        VerticalScrollDecorator {}
        PullDownMenu {
            MenuItem {
                text: qsTr("remove server profile")
                onClicked: {
                    pageStack.pop();
                    deleteProfile(index);

                }
            }
        }

        Column
        {
            id: settingsContent;
            width: parent.width
            DialogHeader {
                title: qsTr("edit profile");
            }
            TextField
            {
                width: parent.width
                id: profilenameInputField
                label: qsTr("Profile name")
                placeholderText:  label
                inputMethodHints: Qt.ImhNoPredictiveText
            }
            TextField
            {
                width: parent.width
                id: hostnameInputField
                label: qsTr("Hostname or IP address")
                placeholderText: label
                inputMethodHints: Qt.ImhNoPredictiveText
            }
            TextField
            {
                width: parent.width
                id: portInputField
                label: qsTr("Port")
                placeholderText: label
                text: "6600"
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: portvalidator
            }
            TextField
            {
                width: parent.width
                id: passwordInputField
                label: qsTr("Password")
                placeholderText: label
                inputMethodHints: Qt.ImhNoPredictiveText
                echoMode: TextInput.Password
            }
            TextField
            {
                width: parent.width
                id: streamingPortInputField
                label: qsTr("Streaming port")
                placeholderText: label
                text: "8081"
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: portvalidator
                visible: false
            }

            TextField
            {
                id: macAddressField
                width: parent.width
                label: qsTr("MAC address (optional)")
                placeholderText: label
                labelVisible: true
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
                validator: macValidator
                visible: true
            }

            TextSwitch
            {
                width: parent.width
                id: autoconnectSwitch;
                text: qsTr("autoconnect");
            }
        }
    }


    onAccepted: {
        if(newprofile) {
            newProfile([index,name,hostname,password,port,autoconnect?1:0,macAddressField.text]);
        }
        else {
            changeProfile([index,name,hostname,password,port,autoconnect?1:0,macAddressField.text]);
        }

    }

    IntValidator{
            id:portvalidator
            top: 65536
            bottom: 1
    }

    RegExpValidator
    {
        id: macValidator
        regExp: /^$|[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}/
    }
}
