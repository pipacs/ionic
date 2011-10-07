import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.pipacs.ionic.Book 1.0

Page {
    property Book book: emptyBook

    tools: chaptersTools

    PageHeader {
        id: header
        text: book.title
    }

    Component {
        id: delegate
        Item {
            height: chapterLabel.height
            width: parent.width
            Label {
                id: chapterLabel
                text: modelData.name
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
        model: book.content
        delegate: delegate
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
