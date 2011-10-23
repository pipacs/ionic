import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: importTools
    orientationLock: prefs.orientation

    signal begin(int total)
    signal add(string title)
    signal done(int total)

    PageHeader {id: header; text: "Add Books"}

    Flickable {
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 9
        clip: true
        flickableDirection: Flickable.VerticalFlick
        contentWidth: column.width
        contentHeight: column.height
        id: instructions

        Column {
            id: column
            spacing: 18
            Label {
                font.pixelSize: 32
                font.bold: true
                width: importTools.width - 18
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "Step 1: Find and download books"
            }
            Label {
                font.pixelSize: 32
                textFormat: Text.RichText
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                width: importTools.width - 18
                text: platform.text("booksources.html")
                onLinkActivated: {platform.browse(link)}
            }
            Label {
                font.pixelSize: 32
                font.bold: true
                width: importTools.width - 18
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "Step 2: Import downloaded books"
            }
            Button {
                text: "Import"
                onClicked: {
                    importDialog.indeterminate = true
                    importDialog.messageText = "Gathering books"
                    importDialog.open()
                    bookFinder.find()
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: instructions
    }

    ProgressDialog {
        id: importDialog
        titleText: "Importing Books"
    }

    ToolBarLayout {
        id: importTools
        visible: true
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop()
            }
        }
    }

    Component.onCompleted: {
        bookFinder.begin.connect(begin)
        bookFinder.add.connect(add)
        bookFinder.done.connect(done)
    }

    onBegin: {
        console.log("* ImportPage.onBegin " + total)
        importDialog.indeterminate = false
        importDialog.minimumValue = 0
        importDialog.maximumValue = total
        importDialog.value = 0
    }

    onAdd: {
        console.log("* ImportPage.onAdd " + title)
        importDialog.messageText = title
        importDialog.value = importDialog.value + 1
    }

    onDone: {
        console.log("* ImportPage.onDone " + total)
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
