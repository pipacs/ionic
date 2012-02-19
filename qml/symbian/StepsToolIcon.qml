import QtQuick 1.1
import com.nokia.symbian 1.1

// Simple ToolButton wrapper
ToolButton {
    property string iconId
    property string stockIcon: null

    Component.onCompleted: {
        if (stockIcon === "menu")
            iconSource = "toolbar-menu"
        else if (stockIcon === "add")
            iconSource = "toolbar-add"
        else if (stockIcon === "delete")
            iconSource = "toolbar-delete"
        else if (stockIcon === "home")
            iconSource = "toolbar-home"
        else if (stockIcon === "backwards")
            iconSource = "toolbar-mediacontrol-backwards"
        else if (stockIcon === "pause")
            iconSource ="toolbar-mediacontrol-pause"
        else if (stockIcon === "stop")
            iconSource = "toolbar-mediacontrol-stop"
        else if (stockIcon === "next")
            iconSource = "toolbar-next"
        else if (stockIcon === "refresh")
            iconSource = "toolbar-refresh"
        else if (stockIcon === "settings")
            iconSource = "toolbar-settings"
        else if (stockIcon === "back")
            iconSource = "toolbar-back"
        else if (stockIcon === "dialer")
            iconSource = "toolbar-dialer"
        else if (stockIcon === "list")
            iconSource = "toolbar-list"
        else if (stockIcon === "forward")
            iconSource = "toolbar-mediacontrol-forward"
        else if (stockIcon === "play")
            iconSource = "toolbar-mediacontrol-play"
        else if (stockIcon === "previous")
            iconSource = "toolbar-previous"
        else if (stockIcon === "search")
            iconSource = "toolbar-search"
        else if (stockIcon === "share")
            iconSource = "toolbar-share"
        else if (stockIcon === "up")
            iconSource = "toolbar-previous"
        else if (stockIcon === "down")
            iconSource = "toolbar-next"
        else if (stockIcon === "bookmark")
            iconSource = "toolbar-mediacontrol-stop"
        else if (stockIcon === "jump")
            iconSource = "toolbar-mediacontrol-forward"
        else if (stockIcon !== null)
            iconSource = stockIcon
        else
            iconSource = iconId
    }
}
