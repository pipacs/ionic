import QtQuick 1.1
import "meego"
import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

StepsDialog {
    property alias note: noteField.text
    property alias description: label.text
    property Book book: emptyBook
    property int index: 0

    acceptText: qsTr("OK")
    rejectText: qsTr("Cancel")
    title: qsTr("Ionic")

    content: Flickable {
        id: flickable
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.topMargin: 10
        contentWidth: col2.width
        contentHeight: col2.height
        flickableDirection: Flickable.VerticalFlick
        Column {
            id: col2
            anchors.top: parent.top
            spacing: 10
            StepsLabel {
                id: label
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
            StepsLabel {
                text: qsTr("\nNote:")
            }
            StepsTextField {
                id: noteField
                width: flickable.width - 15
                Keys.onReturnPressed: parent.focus = true
            }
        }
    }
}
