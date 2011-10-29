import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Book 1.0

Item {
    id: progress
    x: 0
    y: -15
    height: 15
    z: 0.7
    width: parent.width
    Rectangle {
        id: left
        x: 0
        y: 0
        width: library.nowReading.lastProgress * parent.width
        height: parent.height
        color: "blue"
        opacity: 0.3
        onWidthChanged: {
            if (slideOutTimer.running) {
                slideOutTimer.restart()
            } else {
                slideOut.stop()
                slideIn.start()
                slideOutTimer.start()
            }
        }
    }
    Rectangle {
        x: left.width
        y: 0
        width: parent.width - left.width
        height: parent.height
        color: "blue"
        opacity: 0.1
    }
    NumberAnimation {
        id: slideIn
        target: progress
        property: "y"
        easing.type: Easing.OutExpo
        from: -15; to: 0
        duration: 100
    }
    NumberAnimation {
        id: slideOut
        target: progress
        property: "y"
        easing.type: Easing.InExpo
        from: 0; to: -15
        duration: 100
    }
    Timer {
        id: slideOutTimer
        interval: 500
        triggeredOnStart: false
        repeat: false
        running: false
        onTriggered: slideOut.restart()
    }
}
