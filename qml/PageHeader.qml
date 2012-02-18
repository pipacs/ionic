// Emulates the page header of some N9 apps

import QtQuick 1.1
import "symbian"

Rectangle {
    id: pageHeader

    property string text: "Page Title"
    property bool clickable: false   // Set to true to show a combobox indicator
    signal clicked

    height: (platform.osName === "harmattan")? 65: 61
    color: "orange"
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right

    StepsLabel {
        anchors.fill: parent
        anchors.leftMargin: 19
        anchors.topMargin: 13
        text: pageHeader.text
        font.pixelSize: (platform.osName === "harmattan")? 32: 28
        color: "white"
        elide: Text.ElideRight
    }

    Image {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 19
        anchors.topMargin: 19
        source: "image://theme/meegotouch-combobox-indicator-inverted"
        visible: pageHeader.clickable
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("* PageHeader clicked")
        }

        // Forward "clicked" signal on completion
        Component.onCompleted: {
            clicked.connect(pageHeader.clicked)
        }
    }
}
