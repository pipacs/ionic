import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Sheet {
    property int part
    property int position
    property alias note: noteText.text
    property Book book: emptyBook
    property int index

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
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "Bookmark at " + Math.floor(book.getProgress(part, position) * 100) + "% of \"" + book.title + "\"\n\nNotes:"
            }
            TextField {
                id: noteText
                anchors {left: parent.left; right: parent.right}
                Keys.onReturnPressed: {parent.focus = true}
            }
        }
    }
}
