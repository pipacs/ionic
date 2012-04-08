import QtQuick 1.1
import com.pipacs.ionic.IapItem 1.0
import "symbian"

StepsPage {
    orientationLock: prefs.orientation
    id: donatePage

    PageHeader {
        id: title
        text: qsTr("Donate to Ionic")
    }

    StepsLabel {
        id: header
        anchors.top: title.bottom
        anchors.margins: 15
        width: parent.width - 15
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        text: qsTr("Donations are accepted via the Nokia Store. Please select your donation:")
    }

    Component {
        id: delegate
        Item {
            height: 111
            width: donatePage.width
            Column {
                spacing: 15
                StepsButton {
                    id: donateButton
                    text: qsTr("Donate ") + price
                    onClicked: iap.purchase(itemId)
                }
                StepsLabel {
                    text: (purchaseCount > 0)? "Donated already. Thank you!": ""
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
        anchors.margins: 30
        clip: true
        focus: true
        model: iap.items
        delegate: delegate
    }

    StepsScrollDecorator {
        flickableItem: listView
    }

    StepsSpinner {
        id: spinner
        text: qsTr("Updating donations from Nokia Store")
    }

    function onItemsUpdated() {
        spinner.running = false
    }

    Component.onCompleted: {
        iap.itemsUpdated.connect(onItemsUpdated)
        if (!iap.items[0].ready) {
            spinner.running = true
            iap.updateItems()
        }
    }

    onBack: appWindow.pageStack.pop()
}
