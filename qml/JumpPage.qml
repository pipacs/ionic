import QtQuick 1.1
import "meego"
import com.pipacs.ionic.Book 1.0

StepsPage {
    PageHeader {
        id: header
        text: book.title
    }

    StepsButtonRow {
        StepsButton {
            id: showChapters
            text: qsTr("Chapters")
        }
        StepsButton {
            id: showBookmarks
            text: qsTr("Bookmarks")
        }
    }

    onBack: appWindow.pageStack.pop()
}
