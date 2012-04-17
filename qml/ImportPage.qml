import QtQuick 1.1
import "meego"
import com.pipacs.ionic.Book 1.0

StepsPage {
    orientationLock: prefs.orientation
    id: importPage

    signal begin(int total)
    signal add(string title)
    signal done(int total)

    PageHeader {
        id: header
        text: qsTr("Add Books")
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
                text: qsTr("Step 1: Find and download")
            }
            StepsLabel {
                font.pixelSize: (platform.osName === "harmattan")? 28: 22
                textFormat: Text.RichText
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                width: parent.width - 18
                text: platform.text("booksources.html")
                onLinkActivated: Qt.openUrlExternally(link)
            }
            StepsLabel {
                font.pixelSize: (platform.osName === "harmattan")? 28: 22
                font.bold: true
                width: parent.width - 18
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: qsTr("Step 2: Import books")
            }
            StepsButton {
                text: qsTr("Import")
                onClicked: {
                    importDialog.indeterminate = true
                    importDialog.messageText = qsTr("Gathering books")
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
        titleText: qsTr("Importing Books")
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
        importDialog.disableClose = true
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
            message = qsTr("No new books")
        } else if (total === 1) {
            message = qsTr("1 book imported")
        } else {
            message = qsTr("") + total + qsTr(" books imported")
        }
        importDialog.messageText = message
        importDialog.disableClose = false
    }

    onBack: appWindow.pageStack.pop()
}
