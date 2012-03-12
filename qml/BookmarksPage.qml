import QtQuick 1.1
import "meego"
import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

StepsPage {
    id: page
    property Book book: emptyBook
    signal bookmarkAdded
    orientationLock: prefs.orientation

    PageHeader {
        id: header
        text: qsTr("Bookmarks")
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
                source: (platform.osName === "harmattan")? "image://theme/meegotouch-list-background-pressed-center": "qrc:/icons/listbg.png"
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
                    editBookmark.description = qsTr("Bookmark at ") + getProgress(bookmark) + qsTr(" in \"") + book.title + qsTr("\"")
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
            stockIcon: "back"
            onClicked: pageStack.pop()
        }
        StepsToolIcon {
            stockIcon: "add"
            onClicked: {
                addBookmark.book = book
                addBookmark.description = qsTr("Bookmark at ") + getProgress(book.lastBookmark) + qsTr(" in \"") + book.title + qsTr("\"")
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
        message: qsTr("Are you sure to delete bookmark at ") + getProgress(bookmark) + qsTr("?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        onDialogAccepted: book.deleteBookmark(bookmark)
    }

    Component.onCompleted: setToolBar(bookmarksTools)

    function getProgress(bookmark) {
        if (!bookmark) {
            return qsTr("0%")
        }
        var progress = Math.floor(book.getProgress(bookmark.part, bookmark.position) * 100.)
        return qsTr("") + progress + qsTr("%")
    }
}

