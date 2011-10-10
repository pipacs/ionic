import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    property Book book: emptyBook
    tools: bookmarksTools

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
                        text: "At " + Math.floor(book.getProgress(book.bookmarks[index].part, book.bookmarks[index].pos) * 100) + "%"
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
                    mainPage.goTo(book.bookmarks[index].part, book.bookmarks[index].pos, "")
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
                    editBookmark.book = book
                    editBookmark.part = book.bookmarks[contextMenu.index].part
                    editBookmark.position = book.bookmarks[contextMenu.index].pos
                    editBookmark.note = book.bookmarks[contextMenu.index].note
                    editBookmark.index = contextMenu.index
                    editBookmark.open()
                }
            }
            MenuItem {
                text: "Delete"
                onClicked: {
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
                addBookmark.part = book.lastBookmark.part
                addBookmark.position = book.lastBookmark.position
                addBookmark.note = ""
                addBookmark.open()
            }
        }
    }

    BookmarkEditor {
        id: addBookmark
        onAccepted: {
            book.addBookmark(book.lastBookmark.part, book.lastBookmark.pos, note)
        }
    }

    BookmarkEditor {
        id: editBookmark
        onAccepted: {
            console.log("* BookmarksPage.editBookmark.onAccepted: Setting " + index + " to \"" + note + "\"")
            book.setBookmarkNote(index, note)
        }
    }
}

