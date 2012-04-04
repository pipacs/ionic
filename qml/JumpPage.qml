import QtQuick 1.1
import "meego"
import com.pipacs.ionic.Book 1.0

// FIXME:
import com.nokia.meego 1.0
import com.nokia.extras 1.0

StepsPage {
    PageHeader {
        id: header
        text: library.nowReading.title
    }

    StepsButtonRow {
        id: tabSelector
        anchors.top: header.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 29

        StepsButton {
            id: showBookmarks
            text: qsTr("Bookmarks")
            onClicked: tabGroup.currentTab = jumpBookmarksPage
        }
        StepsButton {
            id: showChapters
            text: qsTr("Chapters")
            onClicked: tabGroup.currentTab = jumpChaptersPage
        }
    }

    TabGroup {
        id: tabGroup
        anchors {
            left: parent.left
            right: parent.right
            top: tabSelector.bottom
            bottom: parent.bottom
            topMargin: 29
        }
        ChaptersPage {
            id: jumpChaptersPage
        }
        BookmarksPage {
            id: jumpBookmarksPage
            onBookmarkAdded: infoBookmarkAdded.show()
        }
    }

    StepsToolBarLayout {
        id: bookmarksTools
        visible: true
        StepsToolIcon {
            stockIcon: "back"
            onClicked: pageStack.pop()
        }
        StepsToolIcon {
            stockIcon: "add"
            onClicked: jumpBookmarksPage.addBookmark()
            visible: tabGroup.currentTab === jumpBookmarksPage
        }
    }

    Component.onCompleted: {
        setToolBar(bookmarksTools)
        tabGroup.currentTab = jumpBookmarksPage
    }
}
