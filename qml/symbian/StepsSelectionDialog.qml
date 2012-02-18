import QtQuick 1.1
import com.nokia.symbian 1.1

SelectionDialog {
    signal dialogClosed
    signal dialogAccepted
    signal dialogRejected

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
