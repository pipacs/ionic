// Temporarily reveal toolbar on double-tap

import QtQuick 1.1

Item {
    property bool active: false // Double-taps are only handled if active is true
    property StepsPageStackWindow targetWindow // Target window that has the toolbar

    anchors.fill: parent
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onDoubleClicked: {
            if (active) {
                targetWindow.showToolBar = true
                hideTimer.restart()
            }
        }
        onPressed: mouse.accepted = false
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
