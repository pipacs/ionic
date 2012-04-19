import QtQuick 1.1
import com.nokia.meego 1.0

// Simple wrapper for Page
Page {
    property bool active
    property bool showBack: true
    property bool showTools: true
    property int statusInactive: PageStatus.Inactive
    property int statusActivating: PageStatus.Activating
    property int statusActive: PageStatus.Active
    property int statusDeactivating: PageStatus.Deactivating
    property int orientationAutomatic: PageOrientation.Automatic
    property int orientationLockPortrait:PageOrientation.LockPortrait
    property int orientationLockLandscape: PageOrientation.LockLandscape

    signal back

    active: status === PageStatus.Active
    tools: showTools? pageTools: null

    ToolBarLayout {
        id: pageTools
        visible: showTools
        ToolIcon {
            iconId: "toolbar-back";
            onClicked: back()
            visible: showBack
        }
    }

    function setToolBar(newToolBar) {
        pageTools.visible = false
        tools = newToolBar
        return pageTools
    }
}
