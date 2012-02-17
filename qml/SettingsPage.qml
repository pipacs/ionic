import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: settingsTools
    orientationLock: prefs.orientation

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
                // enabled: useVolumeKeys.checked
                onClicked: {prefs.useSwipe = checked}
            }
            CheckBox {
                id: showToolbar
                text: "Show toolbar"
                checked: prefs.showToolBar
                onClicked: {prefs.showToolBar = checked}
            }
            Label {
                width: parent.width
                color: "grey"
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "If the toolbar is hidden, double-tap to reveal it temporarily"
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
            Label {text: "Theme:"}
            ButtonRow {
                Button {
                    text: "Day"
                    checked: prefs.style == "day"
                    onClicked: {prefs.style = "day"; mainPage.setStyle("day")}
                }
                Button {
                    text: "Night"
                    checked: prefs.style == "night"
                    onClicked: {prefs.style = "night"; mainPage.setStyle("night")}
                }
                Button {
                    text: "Sand"
                    checked: prefs.style == "sand"
                    onClicked: {prefs.style = "sand"; mainPage.setStyle("sand")}
                }
            }
            Label {text: "Orientation:"}
            ButtonRow {
                Button {
                    text: "Auto"
                    checked: prefs.orientation == PageOrientation.Automatic
                    onClicked: {prefs.orientation = PageOrientation.Automatic}
                }
                Button {
                    text: "Portrait"
                    checked: prefs.orientation == PageOrientation.LockPortrait
                    onClicked: {prefs.orientation = PageOrientation.LockPortrait}
                }
                Button {
                    text: "Landscape"
                    checked: prefs.orientation == PageOrientation.LockLandscape
                    onClicked: {prefs.orientation = PageOrientation.LockLandscape}
                }
            }
            CheckBox {
                id: preventBlanking
                text: "Prevent display blanking"
                checked: prefs.preventBlanking
                onClicked: {prefs.preventBlanking = checked}
            }
            Label {text: "Brightness:"}
            Slider {
                id: brightness
                property bool firstUpdate: true
                stepSize: 1
                valueIndicatorVisible: true
                minimumValue: 1
                maximumValue: 5
                value: platform.brightness
                onValueChanged: {
                    if (firstUpdate) {
                        // Work around a slider bug: The first valueChanged notification is sent too early
                        firstUpdate = false
                    } else {
                        platform.brightness = value
                    }
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }

    Timer {
        interval: 5000
        running: true
        repeat: true
        onTriggered: {
            if (prefs.preventBlanking) {
                platform.pauseBlanking()
            }
        }
    }

    Component.onCompleted: {
        zoom.value = prefs.zoom
    }
}
