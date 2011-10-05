import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: libraryTools

    PageHeader {
        id: header
        text: "Library"
    }

    Component {
        id: delegate
        Item {
            height: 110
            Row {
                Image {
                    width: 100
                    source: coverUrl
                }
                Text {
                    text: name
                    font.pixelSize: 26
                    font.family: "Nokia Pure Text"
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
