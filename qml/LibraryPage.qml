import QtQuick 1.1
import "symbian"
import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

StepsPage {
    // FIXME: orientationLock: prefs.orientation

    BookPage {id: bookPage}
    ImportPage {id: importPage}

    PageHeader {
        id: header
        text: sortByModel.get(library.sortBy).header
        clickable: true
        onClicked: sortByDialog.open()
    }

    StepsSelectionDialog {
        id: sortByDialog
        titleText: "Sort books by"
        selectedIndex: library.sortBy
        onSelectedIndexChanged: library.sortBy = sortByDialog.selectedIndex

        model: ListModel {
            id: sortByModel
            ListElement {name: "Title"; modelData: "Title"; header: "Library: By title"}
            ListElement {name: "Author"; modelData: "Author"; header: "Library: By author"}
            ListElement {name: "Date added"; modelData: "Date added"; header: "Library: By date added"}
            ListElement {name: "Date last read"; modelData: "Date last read"; header: "Library: By date last read"}
        }
    }

    Component {
        id: delegate
        Item {
            height: Math.max(65, textName.height + textOpened.height + 15)
            width: parent.width

            BorderImage {
                id: background
                anchors.fill: parent
                // Fill page borders
                anchors.leftMargin: -listView.anchors.leftMargin
                anchors.rightMargin: -listView.anchors.rightMargin
                visible: mouseArea.pressed
                source: (platform.osName === "harmattan")? "image://theme/meegotouch-list-background-pressed-center": "qrc:/icons/listbg.png"
            }

            Row {
                Image {
                    width: 53
                    height: 59
                    source: coverUrl
                }
                Column {
                    StepsLabel {
                        id: textName
                        text: name
                    }
                    StepsLabel {
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
            stockIcon: "add"
            onClicked: pageStack.push(importPage)
        }
    }

    Component.onCompleted: setToolBar(libraryTools)
}
