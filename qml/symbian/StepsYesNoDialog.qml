import QtQuick 1.1
import com.nokia.symbian 1.1

QueryDialog {
    property string title
    signal dialogClosed
    signal dialogAccepted
    signal dialogRejected

    height: 190
    titleText: qsTr("Ionic")
    message: title
    buttonTexts: [qsTr("Yes"), qsTr("No")]

    onStatusChanged: {
        if (status === DialogStatus.Closed) {
            dialogClosed()
        }
    }

    onButtonClicked: {
        if (index === 0) {
            dialogAccepted()
        } else {
            dialogRejected()
        }
    }
}
