import QtQuick 1.1
import com.pipacs.ionic.Book 1.0
import "symbian"

StepsPage {
    orientationLock: prefs.orientation
    id: aboutPage

    Flickable {
        anchors.fill: parent
        anchors.margins: 9
        contentWidth: Math.max(parent.width, column.width)
        contentHeight: Math.max(parent.height, column.height)
        flickableDirection: Flickable.VerticalFlick
        id: about
        Column {
            id: column
            width: aboutPage.width
            Image {
                source: "qrc:/ionic80.png"
            }
            StepsLabel {
                width: parent.width - 18
                font.pixelSize: (platform.osName === "harmattan")? 28: 22
                textFormat: Text.RichText
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "<b>Ionic E-Book Reader</b><p>Version " + platform.version + "<p>" + platform.text("about.html")
                onLinkActivated: platform.browse(link)
            }
        }
    }

    StepsScrollDecorator {
        flickableItem: about
    }

    onBack: appWindow.pageStack.pop()
}
