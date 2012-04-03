import QtQuick 1.1
import com.nokia.meego 1.0

// Simple Dialog with a Close button
Dialog {
    property string titleText: ""
    property bool disableClose: false
    id: popup

    title: Column {
        Item {
            id: header
            height: 65
            width: popup.width

            Label {
                anchors.verticalCenter: header.verticalCenter
                anchors.margins: 5
                id: label
                text: titleText
                font.pixelSize: 28
                elide: Text.ElideRight
                color: "white"
            }

            Image {
                id: closeButton
                width: 40
                anchors.verticalCenter: header.verticalCenter
                anchors.right: header.right
                anchors.margins: 5

                opacity: (disableClose || closeButtonArea.pressed)? 0.5: 1.0
                source: "image://theme/icon-m-common-dialog-close"

                MouseArea {
                    id: closeButtonArea
                    anchors.fill: parent
                    onClicked: if (!disableClose) popup.reject()
                }
            }
        }

        Item {
            id: separator
            height: 65
            width: popup.width

            Rectangle {
                height: 2
                width: popup.width
                anchors.topMargin: 15
                anchors.bottomMargin: 15
                border.color: "grey"
                border.width: 1
            }
        }
    }
}
