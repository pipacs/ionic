import QtQuick 1.1
import "symbian"
import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

StepsPage {
    orientationLock: prefs.orientation
    id: page

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

            StepsCheckBox {
                id: useVolumeKeys
                text: "Navigate with volume keys"
                checked: prefs.useVolumeKeys
                onClicked: prefs.useVolumeKeys = checked
            }
            StepsCheckBox {
                id: useSwipe
                text: "Navigate with swipe"
                checked: prefs.useSwipe
                // enabled: useVolumeKeys.checked
                onClicked: prefs.useSwipe = checked
            }
            StepsCheckBox {
                id: showToolbar
                text: "Show toolbar"
                checked: prefs.showToolBar
                onClicked: prefs.showToolBar = checked
            }
            StepsLabel {
                width: parent.width
                color: "grey"
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "If the toolbar is hidden, double-tap to reveal it temporarily"
            }
            StepsLabel {
                text: "Font:"
            }
            StepsButton {
                id: fontButton
                text: prefs.font
                onClicked: fontSelector.open()
            }

            StepsLabel {
                text: "Zoom level:"
            }
            StepsSlider {
                id: zoom
                property bool firstUpdate: true
                width: flickable.width - 15
                stepSize: 10
                valueIndicatorVisible: true
                minimumValue: 80
                maximumValue: 250
                value: prefs.zoom
                property bool firstValueChange: true
                onValueChanged: {
                    // The first valueChanged signal comes too early: work around it
                    if (firstValueChange) {
                        firstValueChange = false
                    } else {
                        prefs.zoom = value
                    }
                }
            }
            StepsLabel {
                text: "Theme:"
            }
            StepsButtonRow {
                id: styleRow
                width: flickable.width - 15
                StepsButton {
                    text: "Day"
                    id: styleButtonDay
                    checked: prefs.style == "day"
                    onClicked: {
                        prefs.style = "day"
                        mainPage.setStyle("day")
                    }
                }
                StepsButton {
                    id: styleButtonNight
                    text: "Night"
                    checked: prefs.style == "night"
                    onClicked: {
                        prefs.style = "night"
                        mainPage.setStyle("night")
                    }
                }
                StepsButton {
                    id: styleButtonSand
                    text: "Sand"
                    checked: prefs.style == "sand"
                    onClicked: {
                        prefs.style = "sand"
                        mainPage.setStyle("sand")
                    }
                }
            }
            StepsLabel {
                text: "Orientation:"
            }
            StepsButtonRow {
                id: orientationRow
                width: flickable.width - 15
                StepsButton {
                    id: orientationButtonAuto
                    text: "Auto"
                    checked: prefs.orientation === page.orientationAutomatic
                    onClicked: prefs.orientation = page.orientationAutomatic
                }
                StepsButton {
                    id: orientationButtonPortrait
                    text: "Portrait"
                    checked: prefs.orientation === page.orientationLockPortrait
                    onClicked: prefs.orientation = page.orientationLockPortrait
                }
                StepsButton {
                    id: orientationButtonLandscape
                    text: "Landscape"
                    checked: prefs.orientation === page.orientationLockLandscape
                    onClicked: prefs.orientation = page.orientationLockLandscape
                }
            }
            StepsCheckBox {
                id: preventBlanking
                visible: platform.osName !== "symbian"
                text: "Prevent display blanking"
                checked: prefs.preventBlanking
                onClicked: prefs.preventBlanking = checked
            }
            StepsLabel {
                visible: platform.osName !== "symbian"
                text: "Brightness:"
            }
            StepsSlider {
                id: brightness
                visible: platform.osName !== "symbian"
                property bool firstUpdate: true
                width: flickable.width - 15
                stepSize: 1
                valueIndicatorVisible: true
                minimumValue: 1
                maximumValue: 5
                value: platform.brightness
                onValueChanged: platform.brightness = value
            }
        }
    }

    StepsScrollDecorator {
        flickableItem: flickable
    }

    FontSelector {
        id: fontSelector
        onAccepted: {
            fontButton.text = fontName
            prefs.font = fontName
        }
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
        // zoom.value = prefs.zoom
    }

    onBack: pageStack.pop()

    onStatusChanged: {
        // Work around ButtonRow selection bug on Symbian: Set checkedButton by forceActiveFocus
        if (platform.osName === "symbian") {
            if (prefs.style === "day")
                styleRow.checkedButton = styleButtonDay
            else if (prefs.style === "night")
                styleRow.checkedButton = styleButtonNight
            else
                styleRow.checkedButton = styleButtonSand
            if (prefs.orientation === page.orientationAutomatic)
                orientationRow.checkedButton = orientationButtonAuto
            else if (prefs.orientation === page.orientationLockLandscape)
                orientationRow.checkedButton = orientationButtonLandscape
            else
                orientationRow.checkedButton = orientationButtonPortrait
        }
    }
}
