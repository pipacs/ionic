import QtQuick 1.1
import com.nokia.symbian 1.1

// A Flickable that can elevate children if they are obscured by the virtual keyboard
Flickable {
    id: flickable

    Timer {
        id: adjuster
        interval: 200
        onTriggered: flickable.adjust()
    }

    Component.onCompleted: inputContext.visibleChanged.connect(adjuster.restart)

    function adjust() {
        if (!inputContext.visible) {
            return
        }

        var focusChild = null
        function findFocusChild(p) {
            if (p["activeFocus"] === true) {
                focusChild = p
            } else {
                for (var i = 0; i < p["children"].length; i++) {
                    findFocusChild(p["children"][i])
                    if (focusChild !== null) {
                        break
                    }
                }
            }
        }
        findFocusChild(flickable)

        if (focusChild === null) {
            return
        }
        var focusChildY = focusChild["y"]
        var focusChildHeight = focusChild["height"]
        if ((flickable.contentY + flickable.height) < (focusChildY + focusChildHeight)) {
            flickable.contentY = focusChildY + focusChildHeight - flickable.height
        }
    }
}
