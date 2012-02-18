import QtQuick 1.1
import com.nokia.symbian 1.1

// Simple wrapper for BusyIndicator
Item {
    property bool running: false
    id: spinner
    visible: running

    Rectangle {
        width: screen.width
        height: screen.height
        id: cover
        opacity: 0
        color: "black"
        MouseArea {
            anchors.fill: parent
        }
    }

    BusyIndicator {
        anchors.centerIn: cover
        width: 150
        height: 150
        running: spinner.running
    }

    Component.onCompleted: {
        var root = spinner.parent;
        while (root.parent) {
            root = root.parent;
        }
        spinner.parent = root;
    }
}

