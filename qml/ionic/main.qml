import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    initialPage: mainPage

    MainPage {
        id: mainPage
    }

    LibraryPage {
        id: libraryPage
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        // autohide: true

        ToolIcon {
            iconId: "toolbar-mediacontrol-previous"
            onClicked: {mainPage.goToPreviousPage()}
        }

        ToolIcon {
            iconId: "toolbar-mediacontrol-next"
            onClicked: {mainPage.goToNextPage()}
        }

        ToolIcon {
            iconId: "toolbar-list"
        }

        ToolIcon {
            iconId: "toolbar-directory"
            onClicked: {myMenu.close(); pageStack.push(libraryPage)}
        }

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {text: qsTr("Settings")}
            MenuItem {text: "About"}
        }
    }
}
