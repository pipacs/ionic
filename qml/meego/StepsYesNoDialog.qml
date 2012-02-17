import QtQuick 1.1
import com.nokia.meego 1.0

// Simple wrapper for Yes/No QueryDialog
QueryDialog {
    property string title
    signal dialogClosed
    signal dialogAccepted
    signal dialogRejected

    acceptButtonText: qsTr("Yes")
    rejectButtonText: qsTr("No")
    message: title
    titleText: qsTr("Ionic")
    icon: "/images/steps.png"

    onStatusChanged: {
        if (status === DialogStatus.Closed) {
            dialogClosed()
        }
    }

    onAccepted: {
        dialogAccepted()
    }

    onRejected: {
        dialogRejected()
    }
}
