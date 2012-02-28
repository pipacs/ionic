import QtQuick 1.0
import com.nokia.meego 1.0

// SelectionDialog wrapper
SelectionDialog {
    signal dialogClosed
    signal dialogAccepted
    signal dialogRejected

    onStatusChanged: if (status == DialogStatus.Closed) dialogClosed()
    onAccepted: dialogAccepted()
    onRejected: dialogRejected()
}
