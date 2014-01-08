import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Item {
    anchors.fill: parent
    id: mainCoverItm
    CoverBackground {
        id: coverpage
        anchors.fill: parent

        function recheckActive() {
            if ((status === Cover.Activating || status === Cover.Active)) {
                coverimg.active = true
            } else {
                coverimg.active = false
            }
        }

        OpacityRampEffect {
            sourceItem: toggleImage
            direction: OpacityRamp.BottomToTop
        }

        Item {
            id: toggleImage
            anchors.fill: parent
            ToggleImage {
                id: coverimg
                anchors.fill: parent
                sourceprimary: coverimageurl
                sourcesecondary: artistimageurl
                fillMode: Image.PreserveAspectCrop
            }
        }

        Rectangle {
            visible: (coverimg.sourceprimary != ""
                      || coverimg.sourcesecondary != "")
            anchors.fill: parent
            color: Theme.highlightBackgroundColor
            gradient: Gradient {
                GradientStop {
                    position: 0.6
                    color: Qt.rgba(0.0, 0.0, 0.0, 0.0)
                }
                GradientStop {
                    position: 1.0
                    color: Qt.rgba(0.0, 0.0, 0.0, 1.0)
                }
            }
        }
        Image {
            id: logo
            visible: (coverimg.sourceprimary == ""
                      && coverimg.sourcesecondary == ""
                      && (mTitle === "") )
            source: "qrc:images/pictogram.png"
            anchors.centerIn: parent
        }

        onStatusChanged: {
            recheckActive()
        }


        //    Image{
        //        visible: (stopped||(coverimg.sourceprimary==""&&coverimg.sourcesecondary==""))
        //        anchors.centerIn: coverrpage
        //        source: "qrc:/images/smpc-big.png"
        //        width: coverpage.width-(coverpage.width/3)
        //        height: width
        //        smooth: true
        //        opacity: 0.8
        //    }
        Label {
            id: textLabel
            anchors.centerIn: coverpage
            width: coverpage.width - (2 * listPadding)
            height: (coverpage.height / 3) * 2
            wrapMode: "WordWrap"
            elide: Text.ElideRight
            font.pixelSize: Theme.fontSizeLarge
            style: Text.Raised
            styleColor: Theme.secondaryColor
            horizontalAlignment: Text.AlignHCenter
            text: (mTitle === "" ? "SMPC" : mTitle)
        }

        CoverActionList {
            id: coverAction

            CoverAction {
                iconSource: playbuttoniconsourcecover //"image://theme/icon-cover-pause"
                onTriggered: play()
            }

            CoverAction {
                iconSource: "image://theme/icon-cover-next-song"
                onTriggered: next()
            }
        }
    }
}
