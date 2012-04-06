import QtQuick 1.1
import com.pipacs.ionic.IapItem 1.0
import "symbian"

StepsPage {
    orientationLock: prefs.orientation
    id: donatePage

    StepsLabel {
        id: header
        anchors.margins: 15
        width: parent.width - 15
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        text: qsTr("Donations are accepted via the Nokia Store. Please select your donation:")
    }

    Component {
        id: delegate
        Item {
            height: 140
            width: donatePage.width
            Column {
                spacing: 10
                StepsLabel {
                    id: donateLabel
                    width: donatePage.width - 30
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    text: description
                }
                StepsButton {
                    id: donateButton
                    // width: donatePage.width - 15
                    text: qsTr("Donate ") + price
                    enabled: ready
                    onClicked: {
                        console.log("* Buy!")
                    }
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
        anchors.margins: 15
        clip: true
        focus: true
        model: iap.items
        delegate: delegate
    }

    StepsScrollDecorator {
        flickableItem: listView
    }

    Component.onCompleted: {
        iap.collectItems()
    }

    onBack: appWindow.pageStack.pop()
}
