import QtQuick 1.1
import com.nokia.meego 1.0

// Simple ToolButton wrapper
ToolButton {
    property string stockIcon: null
    Component.onCompleted: {
        if (stockIcon !== null) {
            // FIXME
        }
    }
}
