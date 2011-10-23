import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.pipacs.ionic.Book 1.0

Page {
    tools: aboutTools
    orientationLock: prefs.orientation

    Flickable {
        anchors.fill: parent
        anchors.margins: 9
        contentWidth: Math.max(parent.width, column.width)
        contentHeight: Math.max(parent.height, column.height)
        flickableDirection: Flickable.VerticalFlick
        id: about
        Column {
            id: column
            Row {
                height: 90
                Image {source: "qrc:/ionic80.png"}
            }
            Row {
                Label {
                    font.pixelSize: 32
                    textFormat: Text.RichText
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    width: aboutTools.width - 10
                    text: "<b>Ionic E-Book Reader</b><p>Version " + platform.version + "<p>" + platform.text("about.html")
                    onLinkActivated: {platform.browse(link)}
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: about
    }

    ToolBarLayout {
        id: aboutTools
        visible: true
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop()
            }
        }
    }
}
