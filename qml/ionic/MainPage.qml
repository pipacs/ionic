import QtQuick 1.1
import com.nokia.meego 1.0
import QtWebKit 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    signal nowReadingChanged
    tools: commonTools
    property alias book: bookView.book

    BookView {
        id: bookView
        anchors {top: parent.top; left: parent.left; right: parent.right; bottom: parent.bottom}
        onLoadStarted: {
            console.log("* bookView onLoadStarted")
            spinner.visible = true
            spinner.running = true
        }
        onLoadFinished: {
            console.log("* bookView onLoadFinished")
            spinner.visible = false
            spinner.running = false
        }
        onLoadFailed: {
            console.log("* bookView onLoadFailed")
            spinner.visible = false
            spinner.running = false
        }
    }

    BusyIndicator {
        id: spinner
        platformStyle: BusyIndicatorStyle {size: "large"}
        anchors.centerIn: parent
        focus: false
        visible: false
    }

    Component.onCompleted: {
        library.nowReadingChanged.connect(nowReadingChanged)
        library.setNowReading(library.nowReading)
    }

    onNowReadingChanged: {
        console.log("* MainPage.onNowReadingChanged")
        bookView.book = library.nowReading
        goTo(bookView.book.lastBookmark.part, bookView.book.lastBookmark.pos, "")
    }

    function goToPreviousPage() {
        bookView.goToPreviousPage()
    }

    function goToNextPage() {
        bookView.goToNextPage()
    }

    function goTo(part, targetPos, urlFragment) {
        console.log("* MainPage.goTo")
        bookView.targetPos = targetPos
        bookView.part = part
        bookView.urlFragment = urlFragment
        bookView.url = bookView.book.urlFromPart(part)
    }
}
