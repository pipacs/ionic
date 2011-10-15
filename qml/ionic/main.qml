import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

PageStackWindow {
    id: appWindow
    initialPage: mainPage
    showStatusBar: false

    MainPage {id: mainPage}
    LibraryPage {id: libraryPage}
    ChaptersPage {id: chaptersPage}
    AboutPage {id: aboutPage}
    BookPage {id: thisBookPage; enableJump: false}
    SettingsPage {id: settingsPage}

    BookmarksPage {
        id: bookmarksPage
        onBookmarkAdded: {
            // FIXME: infoBanner's default location is wrong if the system status bar is shown
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
                myMenu.close()
                mainPage.goToPreviousPage()
            }
        }
        ToolIcon {
            iconId: "toolbar-down" // toolbar-next"
            enabled: library.nowReading.valid
            onClicked: {
                myMenu.close()
                mainPage.goToNextPage()
            }
        }
        ToolIcon {
            iconId: "toolbar-list"
            enabled: library.nowReading.valid
            onClicked: {
                myMenu.close()
                chaptersPage.book = library.nowReading
                pageStack.push(chaptersPage)
            }
        }
        ToolIcon {
            iconId: "toolbar-favorite-mark"
            enabled: library.nowReading.valid
            onClicked: {
                myMenu.close()
                bookmarksPage.book = library.nowReading
                pageStack.push(bookmarksPage)
            }
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined)? undefined: parent.right
            onClicked: (myMenu.status == DialogStatus.Closed)? myMenu.open(): myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "Book details"
                enabled: library.nowReading.valid
                onClicked: {
                    myMenu.close()
                    thisBookPage.book = library.nowReading
                    pageStack.push(thisBookPage)
                }
            }
            MenuItem {
                text: "Library"
                onClicked: {myMenu.close(); pageStack.push(libraryPage)}
            }
            MenuItem {
                text: "Settings"
                onClicked: {myMenu.close(); pageStack.push(settingsPage)}
            }
            MenuItem {
                text: "About"
                onClicked: {myMenu.close(); pageStack.push(aboutPage)}
            }
        }
    }
}
