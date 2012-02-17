import QtQuick 1.1
import com.nokia.symbian 1.0

// Simple wrapper for ContextMenu
contextMenu {
    property int statusOpening: DialogStatus.Opening
    property int statusOpen: DialogStatus.Open
    property int statusClosing: DialogStatus.Closing
    property int statusClosed: DialogStatus.Closed
}
