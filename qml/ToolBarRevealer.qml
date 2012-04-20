// Temporarily reveal toolbar on double-tap.
// Additionally, display Settings on press-and-hold, but only if book navigation by swipe is disabled.

import QtQuick 1.1

Item {
    id: revealer
    property bool active: false // Double-taps are only handled if active is true
    property StepsPageStackWindow targetWindow // Target window that has the toolbar
    anchors.fill: parent

    MouseArea {
        anchors.fill: parent

        onDoubleClicked: {
            console.log("* ToolBarRevealer: onDoubleClicked")
            if (active) {
                targetWindow.showToolBar = true
                hideTimer.restart()
            }
        }
        onPressed: {
            console.log("* ToolBarRevealer: onPressed")
            // We can't accept press events, otherwise the WebView won't emit linkActivated signals
            mouse.accepted = false
        }
        onReleased: {
            console.log("* ToolBarRevealer: onReleased")
        }
    }

    Timer {
        id: hideTimer
        interval: 3000
        running: false
        repeat: false
        onTriggered: {
            if (active && !prefs.showToolBar) {
                targetWindow.showToolBar = false
            }
        }
    }
}
