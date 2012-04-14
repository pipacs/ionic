// Temporarily reveal toolbar on double-tap.
// Additionally, display Settings on press-and-hold, but only if book navigation by swipe is disabled.

import QtQuick 1.1

Item {
    id: revealer
    property bool active: false // Double-taps are only handled if active is true
    property StepsPageStackWindow targetWindow // Target window that has the toolbar
    signal clicked

    anchors.fill: parent
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: revealer.clicked()
        onDoubleClicked: {
            if (active) {
                targetWindow.showToolBar = true
                hideTimer.restart()
            }
        }
        onPressed: mouse.accepted = (prefs.usePressAndHold && !prefs.useSwipe)
        onPressAndHold: pageStack.push(settingsPage)
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
