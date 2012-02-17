import QtQuick 1.1
import com.nokia.meego 1.0

// Simple wrapper for PageStackWindow
PageStackWindow {
    property bool toolBar: true

    focus: true
    showStatusBar: false
    showToolBar: toolBar

    Component.onCompleted: {
        // Enforce black theme
        theme.inverted = true
    }
}
