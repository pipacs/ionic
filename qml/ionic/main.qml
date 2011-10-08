import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    initialPage: mainPage

    MainPage {id: mainPage}
    LibraryPage {id: libraryPage}
    ChaptersPage {id: chaptersPage}
    AboutPage {id: aboutPage}
    BookPage {id: thisBookPage; enableJump: false}
    BookmarksPage {id: bookmarksPage}

    ToolBarLayout {
        id: commonTools
        visible: true

        ToolIcon {
            iconId: "toolbar-previous"
            enabled: library.nowReading.valid
            onClicked: {
                myMenu.close()
                mainPage.goToPreviousPage()
            }
        }
        ToolIcon {
            iconId: "toolbar-next"
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
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "This book"
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
            // MenuItem {text: "Settings"}
            MenuItem {
                text: "About"
                onClicked: {myMenu.close(); pageStack.push(aboutPage)}
            }
        }
    }
}
