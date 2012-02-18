import QtQuick 1.1
import "meego"

StepsPopup {

    property alias indeterminate: progressBar.indeterminate
    property alias minimumValue: progressBar.minimumValue
    property alias maximumValue: progressBar.maximumValue
    property alias value: progressBar.value
    property alias titleText: titleLabel.text
    property alias messageText: messageLabel.text

    title: Item {
        id: titleField
        height: platformStyle.titleBarHeight
        width: parent.width
        Image {
            id: supplement
            source: "qrc:/ionic80.png"
            height: parent.height - 10
            width: 75
            fillMode: Image.PreserveAspectFit
            anchors.leftMargin: 5
            anchors.rightMargin: 5
        }
        StepsLabel {
            id: titleLabel
            anchors.left: supplement.right
            anchors.verticalCenter: titleField.verticalCenter
            font.capitalization: Font.MixedCase
            color: "white"
            text: "Importing books"
        }
        Image {
            id: closeButton
            anchors.verticalCenter: titleField.verticalCenter
            anchors.right: titleField.right
            source: "image://theme/icon-m-common-dialog-close"
            MouseArea {
                id: closeButtonArea
                anchors.fill: parent
                onClicked: {reject()}
            }
        }
    }

    content: Item {
        id: name
        width: parent.width
        Column {
            StepsLabel {
                id: messageLabel
                height: importDialog.platformStyle.titleBarHeight
                color: "white"
                text: "Gathering books"
                font.pixelSize: 24
                elide: Text.ElideRight
                verticalAlignment: Text.AlignBottom
            }
            StepsProgressBar {
                id: progressBar
                width: name.width
                indeterminate: true
            }
        }
    }
}
