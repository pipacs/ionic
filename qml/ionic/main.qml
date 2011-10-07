import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    initialPage: mainPage

    MainPage {id: mainPage}
    LibraryPage {id: libraryPage}
    ChaptersPage {id: chaptersPage}
    AboutPage {id: aboutPage}

    ToolBarLayout {
        id: commonTools
        visible: true

        ToolIcon {
            iconId: "toolbar-previous"
            onClicked: {
                myMenu.close()
                mainPage.goToPreviousPage()
            }
        }
        ToolIcon {
            iconId: "toolbar-next"
            onClicked: {
                myMenu.close()
                mainPage.goToNextPage()
            }
        }
        ToolIcon {
            iconId: "toolbar-list"
            onClicked: {
                myMenu.close()
                chaptersPage.book = library.nowReading
                pageStack.push(chaptersPage)
            }
        }
        ToolIcon {
            iconId: "toolbar-favorite-mark"
            onClicked: {
                myMenu.close()
            }
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
            // MenuItem {text: "Settings"}
            MenuItem {
                text: "About"
                onClicked: {myMenu.close(); pageStack.push(aboutPage)}
            }
        }
    }
}
