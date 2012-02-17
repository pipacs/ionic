import QtQuick 1.1
import com.nokia.meego 1.0

// Simple wrapper for Slider
Slider {
    property bool changed: false
    property bool firstValueChange: true
    onValueChanged: {
        // The first valueChanged signal comes too early: work around it
        if (firstValueChange) {
            firstValueChange = false
        } else {
            changed = true
        }
    }
}
