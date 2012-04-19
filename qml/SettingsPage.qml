import QtQuick 1.1
import "meego"
import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

StepsPage {
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
                text: qsTr("Navigate with volume keys")
                checked: prefs.useVolumeKeys
                onClicked: prefs.useVolumeKeys = !prefs.useVolumeKeys
            }
            StepsCheckBox {
                id: useSwipe
                text: qsTr("Navigate with swipe")
                checked: prefs.useSwipe
                onClicked: prefs.useSwipe = !prefs.useSwipe
            }
            StepsCheckBox {
                id: useTap
                text: qsTr("Navigate by tapping")
                checked: prefs.useTap
                enabled: !useSwipe.checked
                onClicked: prefs.useTap = !prefs.useTap
            }
            StepsLabel {
                width: flickable.width - 15
                color: "grey"
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: qsTr("Tap the bottom of the screen to go to the next page, tap the top to go to the previous. Only works if <i>Navigate with swipe</i> is disabled")
            }
            StepsCheckBox {
                id: usePressAndHold
                text: qsTr("Long-tap to show Settings")
                checked: prefs.usePressAndHold
                enabled: !useSwipe.checked
                onClicked: prefs.usePressAndHold = !prefs.usePressAndHold
            }
            StepsLabel {
                width: flickable.width - 15
                color: "grey"
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: qsTr("Tap and hold to show <i>Settings</i> directly. Only works if <i>Navigate with swipe</i> is disabled")
            }
            StepsCheckBox {
                id: showToolbar
                text: qsTr("Show toolbar")
                checked: prefs.showToolBar
                onClicked: prefs.showToolBar = !prefs.showToolBar
            }
            StepsLabel {
                width: flickable.width - 15
                color: "grey"
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: qsTr("If the toolbar is hidden, double-tap to reveal it temporarily")
            }
            StepsLabel {
                text: qsTr("Font:")
            }
            StepsButton {
                id: fontButton
                text: prefs.font
                onClicked: fontSelector.open()
            }
            StepsLabel {
                text: qsTr("Zoom level:")
            }
            StepsSlider {
                id: zoom
                width: flickable.width - 15
                stepSize: 10
                valueIndicatorVisible: true
                minimumValue: 80
                maximumValue: 250
                value: prefs.zoom
            }
            StepsLabel {
                text: qsTr("Margins:")
            }
            StepsSlider {
                id: margin
                width: flickable.width - 15
                stepSize: 1
                valueIndicatorVisible: true
                minimumValue: 0
                maximumValue: 45
                value: prefs.margin
            }
            StepsLabel {
                text: qsTr("Theme:")
            }
            StepsButtonRow {
                id: styleRow
                width: flickable.width - 15
                StepsButton {
                    text: qsTr("Day")
                    id: styleButtonDay
                    checked: prefs.style == "day"
                    onClicked: {
                        prefs.style = "day"
                        mainPage.setStyle("day")
                    }
                }
                StepsButton {
                    id: styleButtonNight
                    text: qsTr("Night")
                    checked: prefs.style == "night"
                    onClicked: {
                        prefs.style = "night"
                        mainPage.setStyle("night")
                    }
                }
                StepsButton {
                    id: styleButtonSand
                    text: qsTr("Sand")
                    checked: prefs.style == "sand"
                    onClicked: {
                        prefs.style = "sand"
                        mainPage.setStyle("sand")
                    }
                }
            }
            StepsLabel {
                text: qsTr("Orientation:")
            }
            StepsButtonRow {
                id: orientationRow
                width: flickable.width - 15
                StepsButton {
                    id: orientationButtonAuto
                    text: qsTr("Auto")
                    checked: prefs.orientation === page.orientationAutomatic
                    onClicked: prefs.orientation = page.orientationAutomatic
                }
                StepsButton {
                    id: orientationButtonPortrait
                    text: qsTr("Portrait")
                    checked: prefs.orientation === page.orientationLockPortrait
                    onClicked: prefs.orientation = page.orientationLockPortrait
                }
                StepsButton {
                    id: orientationButtonLandscape
                    text: qsTr("Landscape")
                    checked: prefs.orientation === page.orientationLockLandscape
                    onClicked: prefs.orientation = page.orientationLockLandscape
                }
            }
            StepsCheckBox {
                id: openExternal
                text: qsTr("Open external links with browser")
                checked: prefs.openExternal
                onClicked: prefs.openExternal = checked
            }

            StepsCheckBox {
                id: preventBlanking
                visible: platform.osName !== "symbian"
                text: qsTr("Prevent display blanking")
                checked: prefs.preventBlanking
                onClicked: prefs.preventBlanking = checked
            }
            StepsLabel {
                visible: platform.osName !== "symbian"
                text: qsTr("Brightness:")
            }
            StepsSlider {
                id: brightness
                visible: platform.osName !== "symbian"
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

    onBack: {
        prefs.zoom = zoom.value
        prefs.margin = margin.value
        mainPage.setMargin(margin.value)
        pageStack.pop()
    }

    onStatusChanged: {
        // Work around ButtonRow selection bug on Symbian: Set checkedButtons by force
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
