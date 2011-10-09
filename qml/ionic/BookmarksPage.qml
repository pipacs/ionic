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
                        text: "At " + Math.floor(book.getProgress(part, book.bookmarks[index].pos) * 100) + "%"
                    }
                    Label {
                        font.pixelSize: 24
                        text: note
                    }
                }
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    console.log("* BookmarksPage.delegate.onClicked " + index)
                    pageStack.pop(null)
                    mainPage.goTo(book.bookmarks[index].part, book.bookmarks[index].pos, "")
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
                addBookmark.open()
            }
        }
    }

    Sheet {
        id: addBookmark

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
                    text: "Bookmark at " + Math.floor(book.getProgress(book.lastBookmark.part, book.lastBookmark.pos) * 100) + "% of \"" + book.title + "\"\n\nNotes:"
                }
                TextField {
                    id: note
                    anchors {left: parent.left; right: parent.right}
                    // placeholderText: ""
                    Keys.onReturnPressed: {parent.focus = true}
                }
            }
        }

        onAccepted: {
            book.addBookmark(book.lastBookmark.part, book.lastBookmark.pos, note.text)
        }
    }
}
