import QtQuick 1.1
import "meego"

StepsPageStackWindow {
    id: appWindow
    initialPage: mainPage
    showStatusBar: false
    showToolBar: prefs.showToolbar
    focus: true

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

    StepsBanner {
        id: infoBookmarkAdded
        width: parent.width - 17
        text: "Bookmarked current position"
    }

    StepsToolBarLayout {
        id: commonTools
        visible: true

        StepsToolIcon {
            iconId: "toolbar-up" // toolbar-previous"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                mainPage.goToPreviousPage()
            }
        }
        StepsToolIcon {
            iconId: "toolbar-down" // toolbar-next"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                mainPage.goToNextPage()
            }
        }
        StepsToolIcon {
            iconId: "toolbar-list"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                chaptersPage.book = library.nowReading
                pageStack.push(chaptersPage)
            }
        }
        StepsToolIcon {
            iconId: "toolbar-favorite-mark"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                bookmarksPage.book = library.nowReading
                pageStack.push(bookmarksPage)
            }
        }
        StepsToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined)? undefined: parent.right
            onClicked: {
                if (menu.status === menu.statusClosed) {
                    mainPage.toolBarRevelaerActive = false
                    menu.open()
                } else {
                    menu.close()
                }
            }
        }
    }

    StepsMenu {
        id: menu
        visualParent: pageStack
        StepsMenuLayout {
            StepsMenuItem {
                text: "Book details"
                enabled: library.nowReading.valid
                onClicked: {
                    menu.close()
                    thisBookPage.book = library.nowReading
                    pageStack.push(thisBookPage)
                }
            }
            StepsMenuItem {
                text: "Add books"
                onClicked: {menu.close(); pageStack.push(importPage)}
            }
            StepsMenuItem {
                text: "Library"
                onClicked: {menu.close(); pageStack.push(libraryPage)}
            }
            StepsMenuItem {
                text: "Settings"
                onClicked: {menu.close(); pageStack.push(settingsPage)}
            }
            StepsMenuItem {
                text: "About"
                onClicked: {menu.close(); pageStack.push(aboutPage)}
            }
        }
        onStatusChanged: {
            if (status === statusClosed && mainPage.status === PageStatus.Active) {
                mainPage.toolBarRevelaerActive = true
                showToolBar = prefs.showToolBar
            }
        }
    }
}
