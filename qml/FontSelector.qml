import QtQuick 1.1
import "meego"

StepsPopup {
    property string fontName
    titleText: "Select font"
    anchors.top: title.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

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
