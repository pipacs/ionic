import QtQuick 1.1
import com.pipacs.ionic.Book 1.0
import "meego"

StepsPage {
    id: aboutPage

    Flickable {
        id: about
        anchors.fill: parent
        anchors.margins: 9
        contentWidth: Math.max(parent.width, column.width)
        contentHeight: Math.max(parent.height, column.height)
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: column
            width: aboutPage.width
            spacing: 9

            Image {
                anchors.horizontalCenter: column.horizontalCenter
                source: "qrc:/ionic80.png"
            }

            StepsLabel {
                width: parent.width - 18
                font.pixelSize: (platform.osName == "harmattan")? 28: 22
                textFormat: Text.RichText
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "<br><b>Ionic E-Book Reader, Version " + platform.version + "</b><p>" + platform.text("donate.html")
                onLinkActivated: Qt.openUrlExternally(link)
            }

            StepsButton {
                id: donateButton
                anchors.margins: 0
                anchors.horizontalCenter: column.horizontalCenter
                text: qsTr("Donate")
                onClicked: {
                    if (platform.osName !== "symbian") {
                        Qt.openUrlExternally("file:///opt/ionic/share/donate-paypal.html")
                    } else {
                        pageStack.push(Qt.resolvedUrl("DonatePage.qml"))
                    }
                }
            }

            StepsLabel {
                width: parent.width - 18
                font.pixelSize: (platform.osName == "harmattan")? 28: 22
                textFormat: Text.RichText
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: platform.text("about.html")
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }

    StepsScrollDecorator {
        flickableItem: about
    }

    onBack: appWindow.pageStack.pop()
}
