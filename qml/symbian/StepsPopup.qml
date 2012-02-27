import QtQuick 1.1
import com.nokia.symbian 1.1

// Simple wrapper for Dialog
CommonDialog {
    property bool disableClose: false
    buttonTexts: ["Close"]
    onDisableCloseChanged: buttons[0].enabled = !disableClose
}
