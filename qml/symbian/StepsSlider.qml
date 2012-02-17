import QtQuick 1.1
import com.nokia.symbian 1.1

// Simple wrapper for Slider
Slider {
    property bool changed: false
    onValueChanged: {
        changed = true
    }
}
