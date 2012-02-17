import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Sheet {
    property alias note: noteField.text
    property alias description: label.text
    property Book book: emptyBook
    property int index: 0

    acceptButtonText: "OK"
    rejectButtonText: "Cancel"

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
            Label {
                id: label
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
            Label {
                text: "\nNote:"
            }
            TextField {
                id: noteField
                anchors {left: parent.left; right: parent.right}
                Keys.onReturnPressed: {parent.focus = true}
            }
        }
    }
}
