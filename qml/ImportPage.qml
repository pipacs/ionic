import QtQuick 1.1
import "symbian"
import com.pipacs.ionic.Book 1.0

StepsPage {
    orientationLock: prefs.orientation
    id: importPage

    signal begin(int total)
    signal add(string title)
    signal done(int total)

    PageHeader {
        id: header
        text: "Add Books"
    }

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
            width: importPage.width
            StepsLabel {
                font.pixelSize: (platform.osName === "harmattan")? 28: 22
                font.bold: true
                width: parent.width - 18
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "Step 1: Find and download"
            }
            StepsLabel {
                font.pixelSize: (platform.osName === "harmattan")? 28: 22
                textFormat: Text.RichText
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                width: parent.width - 18
                text: platform.text("booksources.html")
                onLinkActivated: {platform.browse(link)}
            }
            StepsLabel {
                font.pixelSize: (platform.osName === "harmattan")? 28: 22
                font.bold: true
                width: parent.width - 18
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "Step 2: Import books"
            }
            StepsButton {
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

    StepsScrollDecorator {
        flickableItem: instructions
    }

    ProgressDialog {
        id: importDialog
        titleText: "Importing Books"
    }

    Component.onCompleted: {
        bookFinder.begin.connect(begin)
        bookFinder.add.connect(add)
        bookFinder.done.connect(done)
    }

    onBegin: {
        importDialog.indeterminate = false
        importDialog.minimumValue = 0
        importDialog.maximumValue = total
        importDialog.value = 0
        importDialog.buttons[0].enabled = false
    }

    onAdd: {
        importDialog.messageText = title
        importDialog.value = importDialog.value + 1
    }

    onDone: {
        importDialog.maximumValue = 1
        importDialog.value = 1
        var message
        if (total === 0) {
            message = "No new books"
        } else if (total === 1) {
            message = "1 book imported"
        } else {
            message = "" + total + " books imported"
        }
        importDialog.messageText = message
        importDialog.buttons[0].enabled = true
    }

    onBack: appWindow.pageStack.pop()
}
