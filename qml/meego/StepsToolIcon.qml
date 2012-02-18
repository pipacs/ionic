import QtQuick 1.1
import com.nokia.meego 1.0

// Simple ToolIcon wrapper
ToolIcon {
    property string stockIcon: null
    Component.onCompleted: {
        if (stockIcon === "menu")
            iconId = "toolbar-view-menu"
        else if (stockIcon === "delete")
            iconId = "toolbar-delete"
        else if (stockIcon === "home")
            iconId = "toolbar-home"
        else if (stockIcon === "backwards")
            iconId = "toolbar-mediacontrol-backwards"
        else if (stockIcon === "pause")
            iconId ="toolbar-mediacontrol-pause"
        else if (stockIcon === "stop")
            iconId = "toolbar-mediacontrol-stop"
        else if (stockIcon === "next")
            iconId = "toolbar-next"
        else if (stockIcon === "refresh")
            iconId = "toolbar-refresh"
        else if (stockIcon === "settings")
            iconId = "toolbar-settings"
        else if (stockIcon === "back")
            iconId = "toolbar-back"
        else if (stockIcon === "dialer")
            iconId = "toolbar-dialer"
        else if (stockIcon === "list")
            iconId = "toolbar-list"
        else if (stockIcon === "forward")
            iconId = "toolbar-mediacontrol-forward"
        else if (stockIcon === "play")
            iconId = "toolbar-mediacontrol-play"
        else if (stockIcon === "previous")
            iconId = "toolbar-previous"
        else if (stockIcon === "search")
            iconId = "toolbar-search"
        else if (stockIcon === "share")
            iconId = "toolbar-share"
        else if (stockIcon === "up")
            iconId = "toolbar-up"
        else if (stockIcon === "down")
            iconId = "toolbar-down"
        else if (stockIcon === "bookmark")
            iconId = "toolbar-favorite-mark"
        else if (stockIcon === "jump")
            iconId = "toolbar-jump-to"
    }
}
