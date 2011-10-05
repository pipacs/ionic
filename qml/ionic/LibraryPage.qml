import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: libraryTools

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
            height: 120
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
                    width: 70
                    height: 120
                    source: coverUrl
                }
                Column {
                    Text {
                        text: name
                        font.pixelSize: 26
                        font.family: "Nokia Pure Text"
                    }
                    Text {
                        text: "Last read: " + dateOpened
                        font.pixelSize: 20
                        font.family: "Nokia Pure Text"
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

    ToolBarLayout {
        id: libraryTools
        visible: true
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {myMenu.close(); pageStack.pop()}
        }
    }
}
