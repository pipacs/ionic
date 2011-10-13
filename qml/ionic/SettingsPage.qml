import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: settingsTools

    ToolBarLayout {
        id: settingsTools
        visible: true
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop()
            }
        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        anchors.margins: 5
        anchors.topMargin: 15
        anchors.leftMargin: 15
        contentWidth: column.width
        contentHeight: column.height
        flickableDirection: Flickable.VerticalFlick
        clip: true

        Column {
            id: column
            spacing: 30

            CheckBox {
                id: useVolumeKeys
                text: "Navigate with volume keys"
                checked: prefs.useVolumeKeys
                onClicked: {prefs.useVolumeKeys = checked}
            }
            CheckBox {
                id: useSwipe
                text: "Navigate with swipe"
                checked: prefs.useSwipe
                enabled: useVolumeKeys.checked
                onClicked: {prefs.useSwipe = checked}
            }
            Label {text: "Zoom level:"}
            Slider {
                id: zoom
                property bool firstUpdate: true
                stepSize: 10
                valueIndicatorVisible: true
                minimumValue: 80
                maximumValue: 250
                value: prefs.zoom
                onValueChanged: {
                    if (firstUpdate) {
                        // Work around a slider bug: The first valueChanged notification is sent too early
                        firstUpdate = false
                    } else {
                        prefs.zoom = value
                    }
                }
            }
            Label {text: "Style:"}
            ButtonRow {
                Button {
                    text: "Day"
                    checked: prefs.style == "day"
                    onClicked: {prefs.style = "day"}
                }
                Button {
                    text: "Night"
                    checked: prefs.style == "night"
                    onClicked: {prefs.style = "night"}
                }
                Button {
                    text: "Sand"
                    checked: prefs.style == "sand"
                    onClicked: {prefs.style = "sand"}
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }

    Component.onCompleted: {
        zoom.value = prefs.zoom
    }
}
