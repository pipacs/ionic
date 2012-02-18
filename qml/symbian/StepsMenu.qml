import QtQuick 1.1
import com.nokia.symbian 1.1

// Simple wrapper for Label
Menu {
    property int statusOpening: DialogStatus.Opening
    property int statusOpen: DialogStatus.Open
    property int statusClosing: DialogStatus.Closing
    property int statusClosed: DialogStatus.Closed
}
