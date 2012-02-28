import QtQuick 1.0
import "meego"

StepsSelectionDialog {
    id: fontSelector
    property string fontName: ""
    titleText: "Select font"

    model: ListModel {
        id: fonts
    }

    Component.onCompleted: {
        var families = Qt.fontFamilies();
        for (var i = 0; i < families.length; i++) {
            var font = families[i]
            fonts.append({"name": font, "modelData": font}) // Meego is using the "name" property, while Symbian is using "modelData"
        }
    }

    onDialogAccepted: fontName = Qt.fontFamilies()[selectedIndex]
}
