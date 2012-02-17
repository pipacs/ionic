import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    id: page
    property Book book: emptyBook
    signal bookmarkAdded
    tools: bookmarksTools
    orientationLock: prefs.orientation

    PageHeader {
        id: header
        text: "Bookmarks"
    }

    Component {
        id: delegate
        Item {
            height: 60
            width: parent.width
            BorderImage {
                id: background
                anchors.fill: parent
                anchors.leftMargin: -listView.anchors.leftMargin
                anchors.rightMargin: -listView.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }
            Row {
                Column {
                    Label {
                        font.pixelSize: 28
                        text: "At " + getProgress(book.bookmarks[index])
                    }
                    Label {
                        font.pixelSize: 24
                        text: book.bookmarks[index].note
                    }
                }
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    pageStack.pop(null)
                    mainPage.goTo(book.bookmarks[index].part, book.bookmarks[index].position, "#")
                }
                onPressAndHold: {
                    contextMenu.index = index
                    contextMenu.open()
                }
            }
        }
    }

    ListView {
        id: listView
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        clip: true
        focus: true
        model: book.bookmarks
        delegate: delegate
    }

    ScrollDecorator {
        flickableItem: listView
    }

    ContextMenu {
        id: contextMenu
        property int index
        MenuLayout {
            MenuItem {
                text: "Edit"
                onClicked: {
                    var bookmark = book.bookmarks[contextMenu.index]
                    editBookmark.book = book
                    editBookmark.index = contextMenu.index
                    editBookmark.description = "Bookmark at " + getProgress(bookmark) + " in \"" + book.title + "\""
                    editBookmark.note = book.bookmarks[contextMenu.index].note
                    editBookmark.open()
                }
            }
            MenuItem {
                text: "Delete"
                onClicked: {
                    deleteQuery.bookmark = book.bookmarks[contextMenu.index]
                    deleteQuery.open()
                }
            }
        }
    }

    ToolBarLayout {
        id: bookmarksTools
        visible: true
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {pageStack.pop()}
        }
        ToolIcon {
            iconId: "toolbar-add"
            onClicked: {
                addBookmark.book = book
                addBookmark.description = "Bookmark at " + getProgress(book.lastBookmark) + " in \"" + book.title + "\""
                addBookmark.open()
            }
        }
    }

    BookmarkEditor {
        id: addBookmark
        onAccepted: {
            book.addBookmark(book.lastBookmark.part, book.lastBookmark.position, note)
            pageStack.pop(null)
            bookmarkAdded()
        }
    }

    BookmarkEditor {
        id: editBookmark
        onAccepted: {
            console.log("* BookmarksPage.editBookmark.onAccepted: Setting " + index + " to \"" + note + "\"")
            book.setBookmarkNote(index, note)
        }
    }

    QueryDialog {
        property Bookmark bookmark
        id: deleteQuery
        icon: "qrc:/ionic80.png"
        titleText: "Delete bookmark"
        message: "Are you sure to delete bookmark at " + getProgress(bookmark) + "?"
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onAccepted: {
            book.deleteBookmark(bookmark)
        }
    }

    function getProgress(bookmark) {
        if (!bookmark) {
            return "0%"
        }
        var progress = Math.floor(book.getProgress(bookmark.part, bookmark.position) * 100.)
        return "" + progress + "%"
    }
}

