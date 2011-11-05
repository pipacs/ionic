import QtQuick 1.1
import com.nokia.meego 1.0
import QtWebKit 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0
import "theme.js" as Theme

Page {
    signal nowReadingChanged
    property alias toolBarRevelaerActive: revealer.active

    tools: commonTools
    orientationLock: prefs.orientation
    focus: true

    BookView {
        id: bookView
        anchors.fill: parent
        focus: true
        onLoadStarted: {
            spinner.visible = true
            spinner.running = true
        }
        onLoadFinished: {
            spinner.visible = false
            spinner.running = false
        }
        onLoadFailed: {
            spinner.visible = false
            spinner.running = false
        }
    }

    // Reading progress indicator
    ReadingProgress {
        id: readingProgress
    }

    BusyIndicator {
        id: spinner
        platformStyle: BusyIndicatorStyle {size: "large"}
        anchors.centerIn: parent
        visible: false
    }

    ToolBarRevealer {
        id: revealer
    }

    Component.onCompleted: {
        library.nowReadingChanged.connect(nowReadingChanged)
        library.setNowReading(library.nowReading)
        revealer.targetWindow = appWindow
    }

    onStatusChanged: {
        if (status == PageStatus.Activating) {
            revealer.active = true
            appWindow.showToolBar = prefs.showToolBar
            focus = true
        } else if (status == PageStatus.Deactivating) {
            revealer.active = false
            appWindow.showToolBar = true
        }
    }

    onNowReadingChanged: {
        console.log("* MainPage.onNowReadingChanged")
        goTo(library.nowReading.lastBookmark.part, library.nowReading.lastBookmark.position, "#")
    }

    // Handle up/down keys
    Keys.onPressed: {
        if ((event.key == Qt.Key_VolumeUp) || (event.key == Qt.Key_Up) || (event.key == Qt.Key_PageUp)) {
            bookView.goToPreviousPage()
        } else if ((event.key == Qt.Key_VolumeDown) || (event.key == Qt.Key_Down) || (event.key == Qt.Key_PageDown)) {
            bookView.goToNextPage()
        }
    }

    function goToPreviousPage() {
        bookView.goToPreviousPage()
    }

    function goToNextPage() {
        bookView.goToNextPage()
    }

    function goTo(part, targetPos, url) {
        console.log("* MainPage.goTo part " + part + ", targetPos " + targetPos + ", url '" + url + "'")
        bookView.targetPos = targetPos
        bookView.part = part
        if (url == "#") {
            url = library.nowReading.urlFromPart(part)
            console.log("*  url from part: " + url)
        }
        if (url == bookView.url) {
            // Force a jump to the new position
            console.log("*  Same as current, forcing local jump")
            bookView.jump()
        } else {
            console.log("*  Loading new url")
            bookView.load(url)
            // BookView itself will force a jump, after loading url
            // Loading an url with a fragment is however not supported by WebView. So we always end up on the top of the page, even if the chapter URL points to somewhere in the middle. See https://bugs.webkit.org/show_bug.cgi?id=48415
        }
    }

    function setStyle(style) {
        readingProgress.color = Theme.progressColor(style)
        bookView.setStyle(style)
    }
}
