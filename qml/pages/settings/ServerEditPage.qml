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
        contentHeight: settingsContent.height + 20;
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
            clip: true
            anchors {
                right: parent.right
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: listPadding
                rightMargin: listPadding
            }
            DialogHeader {
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
                inputMethodHints: Qt.ImhFormattedNumbersOnly
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
            Label{
                visible: false
                anchors.right: parent.right
                anchors.left: parent.left
                text:qsTr("streaming port:")
            }
            TextField
            {
                anchors.right: parent.right
                anchors.left: parent.left
                id: streamingPortInputField
                text: "8081"
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: portvalidator
                visible: false
            }
            Label{
                anchors.right: parent.right
                anchors.left: parent.left
                text:qsTr("mac address:")
            }
            TextField
            {
                anchors.right: parent.right
                anchors.left: parent.left
                id: macAddressField
                placeholderText: "optional for wake on lan"
                labelVisible: true
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
                validator: macValidator
                visible: true
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
        regExp: /[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}:[0-9a-fA-F]{2}/
    }
}
