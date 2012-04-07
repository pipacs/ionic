import QtQuick 1.1

// On Symbian, start with a splash screen
Item {
    id: mainSymbian

    Splash {
        id: splash
    }

    Loader {
        id: loader
        onLoaded: splash.destroy()
    }

    Timer {
        id: timer
        interval: 100
        running: false
        repeat: false
        onTriggered: loader.source = "main.qml"
    }

    Component.onCompleted: {
        splash.activate()
        timer.start()
    }
}
