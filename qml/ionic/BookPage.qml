import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    property Book book: emptyBook
    property alias enableJump: jumpTool.visible

    tools: libraryTools

    PageHeader {
        id: header
        text: book.title
    }

    QueryDialog {
        id: deleteQuery
        icon: "qrc:/ionic80.png"
        titleText: book.title
        message: "Are you sure to delete this book?"
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onAccepted: {
            library.remove(book)
            pageStack.pop()
        }
    }

    Component {
        id: delegate
        Item {
            height: content.length? Math.max(31, textContent.height): 0
            Row {
                Label {
                    id: textLabel
                    text: content.length? label: ""
                }
                Label {
                    id: textContent
                    text: content
                    font.bold: true
                    // FIXME "style: Text.Sunken" gives an error
                    styleColor: "white"
                    // FIXME: Word wrapping doesn't seem to work
                    wrapMode: Text.WordWrap
                }
            }
        }
    }

    property list<QtObject> bookModel: [
        QtObject {property string label: "Title: "; property string content: book.title},
        QtObject {property string label: "Author: "; property string content: book.creatorsString},
        QtObject {property string label: "Date: "; property string content: book.date},
        QtObject {property string label: "Publisher: "; property string content: book.publisher},
        QtObject {property string label: "Date published: "; property string content: book.datePublished},
        QtObject {property string label: "Subject: "; property string content: book.subject},
        QtObject {property string label: "Source: "; property string content: book.source},
        QtObject {property string label: "Rights: "; property string content: book.rights},
        QtObject {property string label: "Added to library: "; property string content: book.dateAdded},
        QtObject {property string label: "Last read: "; property string content: book.dateOpened}
    ]

    Image {
        anchors.top: header.bottom
        anchors.right: parent.right
        anchors.margins: 11
        source: book.coverUrl
        width: 106
        height: 118
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
        model: bookModel
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
            onClicked: {
                pageStack.pop()
            }
        }
        ToolIcon {
            id: jumpTool
            iconId: "toolbar-jump-to"
            onClicked: {
                pageStack.pop(null)
                library.setNowReading(book)
            }
        }
        ToolIcon {
            iconId: "toolbar-delete"
            enabled: book.valid
            onClicked: {
                deleteQuery.open()
            }
        }
    }
}
