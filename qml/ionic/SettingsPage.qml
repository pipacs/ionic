import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import com.pipacs.ionic.Bookmark 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: settingsTools

    PageHeader {
        id: header
        text: "Settings"
    }

    Flickable {
        id: flickable
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        contentWidth: Math.max(parent.width, column.width)
        contentHeight: Math.max(parent.height, column.height)
        flickableDirection: Flickable.VerticalFlick
        clip: true

        Column {
            id: column
            spacing: 5

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
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }

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
}
