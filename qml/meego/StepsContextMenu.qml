import QtQuick 1.1
import com.nokia.meego 1.0

// Simple wrapper for ContextMenu
ContextMenu {
    property int statusOpening: DialogStatus.Opening
    property int statusOpen: DialogStatus.Open
    property int statusClosing: DialogStatus.Closing
    property int statusClosed: DialogStatus.Closed
}
