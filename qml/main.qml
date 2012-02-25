import QtQuick 1.1
import "symbian"

StepsPageStackWindow {
    id: appWindow
    initialPage: mainPage
    showStatusBar: false
    showToolBar: prefs.showToolbar
    focus: true

    MainPage {
        id: mainPage
    }

    BookPage {
        id: thisBookPage
        enableJump: false
    }

    BookmarksPage {
        id: bookmarksPage
        onBookmarkAdded: infoBookmarkAdded.show()
    }

    ChaptersPage {
        id: chaptersPage
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
            stockIcon: "up"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                mainPage.goToPreviousPage()
            }
        }
        StepsToolIcon {
            stockIcon: "down"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                mainPage.goToNextPage()
            }
        }
        StepsToolIcon {
            stockIcon: "list"
            enabled: library.nowReading.valid
            onClicked: {
                menu.close()
                chaptersPage.book = library.nowReading
                pageStack.push(chaptersPage)
            }
        }
        StepsToolIcon {
            stockIcon: (platform.osName === "harmattan")? "bookmark": "qrc:/icons/bookmark.png"
            onClicked: {
                menu.close()
                bookmarksPage.book = library.nowReading
                pageStack.push(bookmarksPage)
            }
        }
        StepsToolIcon {
            stockIcon: "menu"
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
                onClicked: {menu.close(); pageStack.push(Qt.resolvedUrl("ImportPage.qml"))}
            }
            StepsMenuItem {
                text: "Library"
                onClicked: {menu.close(); pageStack.push(Qt.resolvedUrl("LibraryPage.qml"))}
            }
            StepsMenuItem {
                text: "Settings"
                onClicked: {menu.close(); pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))}
            }
            StepsMenuItem {
                text: "About"
                onClicked: {menu.close(); pageStack.push(Qt.resolvedUrl("AboutPage.qml"))}
            }
            StepsMenuItem {
                text: "Exit"
                onClicked: Qt.quit()
                visible: platform.osName === "symbian"
            }
        }
        onStatusChanged: {
            if (status === statusClosed && mainPage.status === mainPage.statusActive) {
                mainPage.toolBarRevelaerActive = true
                showToolBar = prefs.showToolBar
            }
        }
    }

    Splash {
        id: splash
        Component.onCompleted: splash.activate();
        onFinished: splash.destroy();
    }
}
