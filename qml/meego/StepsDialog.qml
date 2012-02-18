import QtQuick 1.1
import com.nokia.meego 1.0

// Simple wrapper for Sheet
Sheet {
    property string title
    property string acceptText
    property string rejectText
    signal dialogClosed
    signal dialogAccepted
    signal dialogRejected

    acceptButtonText: acceptText
    rejectButtonText: rejectText

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
