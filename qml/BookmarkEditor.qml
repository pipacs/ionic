import QtQuick 1.1
import "symbian"
import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

StepsDialog {
    property alias note: noteField.text
    property alias description: label.text
    property Book book: emptyBook
    property int index: 0

    acceptText: "OK"
    rejectText: "Cancel"
    title: "Ionic"

    content: Flickable {
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
                text: "\nNote:"
            }
            StepsTextField {
                id: noteField
                anchors {
                    left: parent.left
                    right: parent.right
                }
                Keys.onReturnPressed: parent.focus = true
            }
        }
    }
}
