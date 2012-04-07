import QtQuick 1.1

Item {
    Splash {
        id: splash
        Component.onCompleted: splash.activate();
    }

    Loader {
        id: loader
    }

    Connections {
        target: loader.item
        onMessage: {
            if (msg === "completed") {
                splash.destroy()
            }
        }
    }

    Component.onCompleted: loader.source = "main.qml"
}
