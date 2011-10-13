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
                checked: settings.useVolumeKeys
            }
            CheckBox {
                id: useSwipe
                text: "Navigate with swipe"
                checked: settings.useSwipe
                enabled: useVolumeKeys.checked
            }
            Label {text: "Zoom level:"}
            Slider {
                id: zoom
                stepSize: 10
                valueIndicatorVisible: true
                minimumValue: 80
                maximumValue: 250
                value: settings.zoom
            }
            Label {text: "Style:"}
            ButtonRow {
                Button {text: "Day"}
                Button {text: "Night"}
                Button {text: "Sand"}
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }
}
