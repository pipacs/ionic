import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: libraryTools
    signal begin(int total)
    signal add(string title)
    signal done(int total)

    BookPage {
        id: bookPage
    }

    PageHeader {
        id: header
        text: "Library"
    }

    Component {
        id: delegate
        Item {
            height: Math.max(65, textName.height + textOpened.height + 11)
            width: parent.width

            BorderImage {
                id: background
                anchors.fill: parent
                // Fill page borders
                anchors.leftMargin: -listView.anchors.leftMargin
                anchors.rightMargin: -listView.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }

            Row {
                Image {
                    width: 53
                    height: 59
                    source: coverUrl
                }
                Column {
                    Label {
                        id: textName
                        text: name
                    }
                    Label {
                        id: textOpened
                        text: "Last read: " + dateOpened
                        font.pixelSize: platformStyle.fontPixelSize - 4
                    }
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    console.log("* Item.onClicked " + index)
                    bookPage.book = library.books[index]
                    pageStack.push(bookPage)
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
        anchors.margins: 11
        clip: true
        focus: true
        model: library.books
        delegate: delegate
    }

    ScrollDecorator {
        flickableItem: listView
    }

    ProgressDialog {
        id: importDialog
        titleText: "Importing Books"
    }

    ToolBarLayout {
        id: libraryTools
        visible: true
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {pageStack.pop()}
        }
        ToolIcon {
            iconId: "toolbar-add"
            onClicked: {
                importDialog.indeterminate = true
                importDialog.messageText = "Gathering books"
                importDialog.open()
                bookFinder.find()
            }
        }
        ToolIcon {
            iconId: "toolbar-search"
        }
    }

    Component.onCompleted: {
        bookFinder.begin.connect(begin)
        bookFinder.add.connect(add)
        bookFinder.done.connect(done)
    }

    onBegin: {
        console.log("* LibraryPage.onBegin " + total)
        importDialog.indeterminate = false
        importDialog.minimumValue = 0
        importDialog.maximumValue = total
        importDialog.value = 0
    }

    onAdd: {
        console.log("* LibraryPage.onAdd " + title)
        importDialog.messageText = title
        importDialog.value = importDialog.value + 1
    }

    onDone: {
        console.log("* LibraryPage.onDone " + total)
        importDialog.maximumValue = 1
        importDialog.value = 1
        var message
        if (total === 0) {
            message = "No new books"
        } else if (total == 1) {
            message = "1 book imported"
        } else {
            message = "" + total + " books imported"
        }
        importDialog.messageText = message
    }
}
