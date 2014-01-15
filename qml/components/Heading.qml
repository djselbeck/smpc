import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: mainItm
    width: (orientation === Orientation.Portrait || orientation === Orientation.PortraitInverted) ? ( parent ? parent.width : 0) : Theme.itemSizeMedium
    height: (orientation === Orientation.Portrait || orientation === Orientation.PortraitInverted ) ? Theme.itemSizeMedium : ( parent ? parent.height : 0)
    property string text
    Label
    {
        id: headerLabel
        text: mainItm.text
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
            rightMargin: listPadding
        }

        font {
            family: Theme.fontFamilyHeading
            pixelSize: Theme.fontSizeLarge
        }
        color: Theme.highlightColor
    }
    Label {
        id: rotateLabel
        text: mainItm.text
        font {
            family: Theme.fontFamilyHeading
            pixelSize: Theme.fontSizeLarge
        }
        color: Theme.highlightColor
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: listPadding
        }

        transform: [
            Rotation {
                origin {
                    x: 0 //( rotateLabel.width / 2 ) - ( rotateLabel.height / 2 )
                    y: 0 // ( rotateLabel.height / 2 ) + ( rotateLabel.width / 2 )
                }

                axis {
                    x: 0.0
                    y: 0.0
                    z: 1.0
                }
                angle: -90
            },
            Translate {
                y: rotateLabel.height
                x: rotateLabel.height / 2
            }
        ]
    }

    states: [
        State {
            name: "landscape"
            PropertyChanges {
                target: headerLabel
                visible: false
            }
            PropertyChanges {
                target: rotateLabel
                visible: true
            }
        },
        State {
            name: "portrait"
            PropertyChanges {
                target: headerLabel
                visible: true
            }
            PropertyChanges {
                target: rotateLabel
                visible: false
            }
        }
    ]
    state: (orientation === Orientation.Portrait || orientation === Orientation.PortraitInverted ) ? "portrait" : "landscape"
}
