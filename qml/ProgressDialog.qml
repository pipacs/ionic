import QtQuick 1.1
import "symbian"

StepsPopup {

    property alias indeterminate: progressBar.indeterminate
    property alias minimumValue: progressBar.minimumValue
    property alias maximumValue: progressBar.maximumValue
    property alias value: progressBar.value
    property alias messageText: messageLabel.text
    titleText: "Importing books"

    content: Item {
        id: name
        anchors.top: title.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        //anchors.bottom: parent.bottom
        anchors.margins: 5
        height: 150
        Column {
            anchors.centerIn: parent
            StepsLabel {
                id: messageLabel
                text: "Gathering books"
                elide: Text.ElideRight
            }
            StepsProgressBar {
                id: progressBar
                width: name.width
                indeterminate: true
            }
        }
    }
}
