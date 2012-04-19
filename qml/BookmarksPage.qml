import QtQuick 1.1
import "meego"
import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

StepsPage {
    id: page
    property Book book: library.nowReading
    signal bookmarkAdded
    showTools: false

    Component {
        id: delegate
        Item {
            height: 71
            width: parent.width
            BorderImage {
                id: background
                anchors.fill: parent
                anchors.leftMargin: -listView.anchors.leftMargin
                anchors.rightMargin: -listView.anchors.rightMargin
                visible: mouseArea.pressed
                source: "qrc:/icons/listbg.png"
            }
            Row {
                Column {
                    StepsLabel {
                        font.pixelSize: 28
                        text: qsTr("At ") + getProgress(book.bookmarks[index]) + qsTr("")
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
                    mainPage.pageStack.pop(null)
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
        anchors.top: page.top
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
                    editBookmarkDialog.book = book
                    editBookmarkDialog.index = contextMenu.index
                    editBookmarkDialog.description = qsTr("Bookmark at ") + getProgress(bookmark) + qsTr(" in \"") + book.title + qsTr("\"")
                    editBookmarkDialog.note = book.bookmarks[contextMenu.index].note
                    editBookmarkDialog.open()
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

    BookmarkEditor {
        id: addBookmarkDialog
        onDialogAccepted: {
            book.addBookmark(book.lastBookmark.part, book.lastBookmark.position, note)
            mainPage.pageStack.pop(null)
            bookmarkAdded()
        }
    }

    BookmarkEditor {
        id: editBookmarkDialog
        onDialogAccepted: {
            console.log("* BookmarksPage.editBookmarkDialog.onAccepted: Setting " + index + " to \"" + note + "\"")
            book.setBookmarkNote(index, note)
        }
    }

    StepsYesNoDialog {
        property Bookmark bookmark
        id: deleteQuery
        icon: "qrc:/ionic80.png"
        message: qsTr("Are you sure to delete bookmark at ") + getProgress(bookmark) + qsTr("?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        onDialogAccepted: book.deleteBookmark(bookmark)
    }

    function getProgress(bookmark) {
        if (!bookmark) {
            return qsTr("0%")
        }
        var progress = Math.floor(book.getProgress(bookmark.part, bookmark.position) * 100.)
        return qsTr("") + progress + qsTr("%")
    }

    function addBookmark () {
        addBookmarkDialog.book = book
        addBookmarkDialog.description = qsTr("Bookmark at ") + getProgress(book.lastBookmark) + qsTr(" in \"") + book.title + qsTr("\"")
        addBookmarkDialog.open()
    }
}

