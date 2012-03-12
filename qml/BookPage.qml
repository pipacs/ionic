import QtQuick 1.1
import "meego"
import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

StepsPage {
    property Book book: emptyBook
    property alias enableJump: jumpTool.visible

    orientationLock: prefs.orientation

    PageHeader {
        id: header
        text: book.title
    }

    StepsYesNoDialog {
        id: deleteQuery
        icon: "qrc:/ionic80.png"
        title: qsTr("Are you sure to delete this book?")
        onDialogAccepted: {
            library.remove(book)
            pageStack.pop()
        }
    }

    Component {
        id: delegate
        Item {
            height: content.length? (column.height + 9): 0
            Column {
                id: column
                StepsLabel {
                    id: textLabel
                    text: content.length? label: ""
                }
                StepsLabel {
                    id: textContent
                    text: content
                    font.bold: true
                    width: libraryTools.width - listView.anchors.leftMargin - listView.anchors.rightMargin - image.width - image.anchors.leftMargin - image.anchors.rightMargin
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
        }
    }

    property list<QtObject> bookModel: [
        QtObject {property string label: qsTr("Title:"); property string content: book.title},
        QtObject {property string label: qsTr("Author:"); property string content: book.creatorsString},
        QtObject {property string label: qsTr("Date:"); property string content: book.date},
        QtObject {property string label: qsTr("Publisher:"); property string content: book.publisher},
        QtObject {property string label: qsTr("Date published:"); property string content: book.datePublished},
        QtObject {property string label: qsTr("Subject:"); property string content: book.subject},
        QtObject {property string label: qsTr("Source:"); property string content: book.source},
        QtObject {property string label: qsTr("Rights:"); property string content: book.rights},
        QtObject {property string label: qsTr("Location:"); property string content: book.path},
        QtObject {property string label: qsTr("Added to library:"); property string content: book.dateAdded},
        QtObject {property string label: qsTr("Last read:"); property string content: book.dateOpened},
                                              QtObject {property string label: qsTr("Reading progress:"); property string content: book.lastProgress? (qsTr("") + Math.floor(book.lastProgress * 100) + qsTr("%")): qsTr("")}
    ]

    Image {
        id: image
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

    StepsScrollDecorator {
        flickableItem: listView
    }

    StepsToolBarLayout {
        id: libraryTools
        visible: true
        StepsToolIcon {
            stockIcon: "back"
            onClicked: pageStack.pop()
        }
        StepsToolIcon {
            id: jumpTool
            stockIcon: "jump"
            onClicked: {
                pageStack.pop(null)
                library.setNowReading(book)
            }
        }
        StepsToolIcon {
            stockIcon: "delete"
            enabled: book.valid
            onClicked: deleteQuery.open()
        }
    }

    Component.onCompleted: setToolBar(libraryTools)
}
