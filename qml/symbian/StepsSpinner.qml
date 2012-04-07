import QtQuick 1.1
import com.nokia.symbian 1.1

// Simple wrapper for BusyIndicator
Item {
    property bool running: false
    property bool darken: true
    property alias text: label.text
    id: spinner
    visible: running

    Rectangle {
        width: screen.width
        height: screen.height
        id: cover
        opacity: darken? 0.7: 0
        color: "black"
        MouseArea {
            anchors.fill: parent
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: cover
        width: 100
        height: 100
        running: spinner.running
    }

    Label {
        id: label
        anchors.horizontalCenter: cover.horizontalCenter
        anchors.top: busyIndicator.bottom
        anchors.margins: 31
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    Component.onCompleted: {
        var root = spinner.parent;
        while (root.parent) {
            root = root.parent;
        }
        spinner.parent = root;
    }
}

