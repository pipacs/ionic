import QtQuick 1.1
import "symbian"
import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

StepsPage {
    id: page
    property Book book: emptyBook
    signal bookmarkAdded
    orientationLock: prefs.orientation

    PageHeader {
        id: header
        text: "Bookmarks"
    }

    Component {
        id: delegate
        Item {
            height: 64
            width: parent.width
            BorderImage {
                id: background
                anchors.fill: parent
                anchors.leftMargin: -listView.anchors.leftMargin
                anchors.rightMargin: -listView.anchors.rightMargin
                visible: mouseArea.pressed
                source: (platform.osName === "meego")? "image://theme/meegotouch-list-background-pressed-center": "qrc:/icons/listbg.png"
            }
            Row {
                Column {
                    StepsLabel {
                        font.pixelSize: 28
                        text: "At " + getProgress(book.bookmarks[index])
                    }
                    StepsLabel {
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

    StepsScrollDecorator {
        flickableItem: listView
    }

    StepsContextMenu {
        id: contextMenu
        property int index
        StepsMenuLayout {
            StepsMenuItem {
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
            StepsMenuItem {
                text: "Delete"
                onClicked: {
                    deleteQuery.bookmark = book.bookmarks[contextMenu.index]
                    deleteQuery.open()
                }
            }
        }
    }

    StepsToolBarLayout {
        id: bookmarksTools
        visible: true
        StepsToolIcon {
            iconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
        StepsToolIcon {
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
        onDialogAccepted: {
            book.addBookmark(book.lastBookmark.part, book.lastBookmark.position, note)
            pageStack.pop(null)
            bookmarkAdded()
        }
    }

    BookmarkEditor {
        id: editBookmark
        onDialogAccepted: {
            console.log("* BookmarksPage.editBookmark.onAccepted: Setting " + index + " to \"" + note + "\"")
            book.setBookmarkNote(index, note)
        }
    }

    StepsYesNoDialog {
        property Bookmark bookmark
        id: deleteQuery
        icon: "qrc:/ionic80.png"
        message: "Are you sure to delete bookmark at " + getProgress(bookmark) + "?"
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onDialogAccepted: book.deleteBookmark(bookmark)
    }

    Component.onCompleted: setToolBar(bookmarksTools)

    function getProgress(bookmark) {
        if (!bookmark) {
            return "0%"
        }
        var progress = Math.floor(book.getProgress(bookmark.part, bookmark.position) * 100.)
        return "" + progress + "%"
    }
}

