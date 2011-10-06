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

        ToolIcon {
            iconId: "toolbar-previous"
            onClicked: {mainPage.goToPreviousPage()}
        }
        ToolIcon {
            iconId: "toolbar-next"
            onClicked: {mainPage.goToNextPage()}
        }
        ToolIcon {
            iconId: "toolbar-list"
        }
        ToolIcon {
            iconId: "toolbar-favorite-mark"
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
            MenuItem {
                text: "Library"
                onClicked: {myMenu.close(); pageStack.push(libraryPage)}
            }
            MenuItem {text: "Settings"}
            MenuItem {text: "About"}
        }
    }
}
