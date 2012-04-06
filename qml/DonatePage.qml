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
            height: 65
            width: donatePage.width
            StepsButton {
                id: donateButton
                // width: donatePage.width - 15
                text: qsTr("Donate ") + price
                enabled: ready
                onClicked: {
                    iap.purchase(itemId)
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

    Component.onCompleted: {
        iap.updateItems()
    }

    onBack: appWindow.pageStack.pop()
}
