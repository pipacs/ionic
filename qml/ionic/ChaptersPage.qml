import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.pipacs.ionic.Book 1.0

Page {
    property Book book: emptyBook

    tools: chaptersTools

    PageHeader {
        id: header
        text: "Chapters: " + book.title
    }

    Component {
        id: delegate
        Item {
            height: chapterLabel.height + 15
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
            Label {
                anchors.verticalCenter: parent.verticalCenter
                id: chapterLabel
                text: modelData
                font.pixelSize: 30
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    console.log("* ChaptersPage.delegate.onClicked " + index)
                    var part = book.partFromChapter(index);
                    var urlFragment = book.fragmentFromChapter(index)
                    console.log("*  Going to part " + part + ", fragment " + urlFragment)
                    pageStack.pop(null)
                    mainPage.goTo(part, 0, urlFragment)
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
        // anchors.margins: platformStyle.margins
        clip: true
        focus: true
        model: book.chapterNames
        delegate: delegate
        // delegate: ListDelegate
    }

    ScrollDecorator {
        flickableItem: listView
    }

    ToolBarLayout {
        id: chaptersTools
        visible: true
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop()
            }
        }
    }
}
