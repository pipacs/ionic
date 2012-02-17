import QtQuick 1.1
import "meego"
import com.pipacs.ionic.Book 1.0

StepsPage {
    property Book book: emptyBook
    orientationLock: prefs.orientation

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
                anchors.leftMargin: -listView.anchors.leftMargin
                anchors.rightMargin: -listView.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }
            StepsLabel {
                width: chaptersTools.width - listView.anchors.leftMargin - listView.anchors.rightMargin
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
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
                    var url = book.urlFromChapter(index)
                    console.log("*  Going to part " + part + ", url " + url)
                    pageStack.pop(null)
                    mainPage.goTo(part, 0, url)
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
        model: book.chapterNames
        delegate: delegate
    }

    StepsScrollDecorator {
        flickableItem: listView
    }

    onBack: appWindow.pageStack.pop()
}
