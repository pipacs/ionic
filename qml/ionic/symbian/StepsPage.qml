import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    property bool active
    property bool showBack: true
    property bool showTools: true
    property bool lockOrientation: true

    signal back

    orientationLock: lockOrientation? PageOrientation.LockPortrait: PageOrientation.Automatic
    active: status === PageStatus.Active

    // FIXME: This is causing problems on Symbian:
    // tools: showTools? pageTools: null
    tools: pageTools

    ToolBarLayout {
        id: pageTools
        ToolButton {
            visible: showBack
            onClicked: back()

            // There is no toolbar-exit icon, so let's stick to toolbar-back for now
            // iconSource: (pageStack.depth > 1)? "toolbar-back": "toolbar-back"
            iconSource: "toolbar-back"
        }
    }

    onStatusChanged: {
        // Enforce focus on activation
        if (status === PageStatus.Active) {
            focus = true
        }
    }

    function setToolBar(newToolBar) {
        pageTools.visible = false
        tools = newToolBar
        return pageTools
    }
}
