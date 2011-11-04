import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

PageStackWindow {
    id: appWindow
    initialPage: mainPage
    showStatusBar: false
    showToolBar: prefs.showToolbar

    MainPage {id: mainPage}
    LibraryPage {id: libraryPage}
    ChaptersPage {id: chaptersPage}
    AboutPage {id: aboutPage}
    BookPage {id: thisBookPage; enableJump: false}
    SettingsPage {id: settingsPage}
    ImportPage {id: importPage}
    BookmarksPage {
        id: bookmarksPage
        onBookmarkAdded: {
            infoBookmarkAdded.show()
        }
    }

    InfoBanner {
        id: infoBookmarkAdded
        width: parent.width - 17
        text: "Bookmarked current position"
    }

    ToolBarLayout {
        id: commonTools
        visible: true

        ToolIcon {
            iconId: "toolbar-up" // toolbar-previous"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                mainPage.goToPreviousPage()
            }
        }
        ToolIcon {
            iconId: "toolbar-down" // toolbar-next"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                mainPage.goToNextPage()
            }
        }
        ToolIcon {
            iconId: "toolbar-list"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                chaptersPage.book = library.nowReading
                pageStack.push(chaptersPage)
            }
        }
        ToolIcon {
            iconId: "toolbar-favorite-mark"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                bookmarksPage.book = library.nowReading
                pageStack.push(bookmarksPage)
            }
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined)? undefined: parent.right
            onClicked: {
                if (menu.status == DialogStatus.Closed) {
                    mainPage.toolBarRevelaerActive = false
                    menu.open()
                } else {
                    menu.close()
                }
            }
        }
    }

    Menu {
        id: menu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "Book details"
                enabled: library.nowReading.valid
                onClicked: {
                    menu.close()
                    thisBookPage.book = library.nowReading
                    pageStack.push(thisBookPage)
                }
            }
            MenuItem {
                text: "Add books"
                onClicked: {menu.close(); pageStack.push(importPage)}
            }
            MenuItem {
                text: "Library"
                onClicked: {menu.close(); pageStack.push(libraryPage)}
            }
            MenuItem {
                text: "Settings"
                onClicked: {menu.close(); pageStack.push(settingsPage)}
            }
            MenuItem {
                text: "About"
                onClicked: {menu.close(); pageStack.push(aboutPage)}
            }
        }
        onStatusChanged: {
            if (status == DialogStatus.Closed && mainPage.status == PageStatus.Active) {
                mainPage.toolBarRevelaerActive = true
                showToolBar = prefs.showToolBar
            }
        }
    }
}
