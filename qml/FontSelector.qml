import QtQuick 1.1
import "symbian"

StepsPopup {
    property string fontName
    buttonTexts: ["Cancel"]
    titleText: "Select font"
    anchors.fill: parent
    content: Flickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        ListView {
            id: listView
            anchors.fill: parent
            anchors.margins: 11
            model: Qt.fontFamilies()

            delegate: Item {
                height: 50
                width: listView.width
                BorderImage {
                    id: background
                    anchors.fill: parent
                    anchors.leftMargin: -listView.anchors.leftMargin
                    anchors.rightMargin: -listView.anchors.rightMargin
                    visible: mouseArea.pressed
                    source: (platform.osName === "harmattan")? "image://theme/meegotouch-list-background-pressed-center": "qrc:/icons/listbg.png"
                }
                StepsLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    text: modelData
                    font.family: modelData
                    font.pixelSize: 28
                }
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        console.log("* FontSelector: " + index + ": " + modelData)
                        fontName = modelData
                        accept()
                    }
                }
            }
        }
    }
}
