# Add more folders to ship with the application, here
folder_01.source = qml/ionic
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE1E990A6

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Add dependency to symbian components
# CONFIG += qtquickcomponents

QT += webkit xml sql network
unix {
    DEFINES += unix
    DEFINES += USE_FILE32API
}
unix:!symbian {
    LIBS += -lz
}
unix:!symbian:!maemo5 {
    # Sorry Dennis
    DEFINES += Q_WS_HARMATTAN
    CONFIG += link_pkgconfig
    CONFIG += qmsystem2
    PKGCONFIG += libresourceqt1
}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    backend/book.cpp \
    backend/bookdb.cpp \
    backend/bookfinder.cpp \
    backend/extractzip.cpp \
    backend/unzip/unzip.c \
    backend/unzip/ioapi.c \
    backend/library.cpp \
    backend/trace.cpp \
    backend/coverprovider.cpp \
    backend/eventfilter.cpp \
    backend/preferences.cpp \
    backend/platform.cpp \
    backend/bookdbworker.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    backend/unzip/MiniZip64_info.txt \
    backend/unzip/MiniZip64_Changes.txt \
    backend/unzip/Makefile \
    backend/unzip/make_vms.com \
    backend/unzip/make_vms_com \
    pkg/version.txt \
    pkg/acknowledgements.txt \
    books/2BR02B.epub \
    icons/view-normal.png \
    icons/view-fullscreen.png \
    icons/system-file-manager.png \
    icons/style-sand.png \
    icons/style-night.png \
    icons/style-default.png \
    icons/style-day.png \
    icons/splash.png \
    icons/splash.jpg \
    icons/settings-portrait.png \
    icons/settings-landscape.png \
    icons/search.png \
    icons/rotate.png \
    icons/previous.png \
    icons/previous-disabled.png \
    icons/preferences-system.png \
    icons/next.png \
    icons/next-disabled.png \
    icons/library.png \
    icons/info.png \
    icons/goto.png \
    icons/general_delete.png \
    icons/forward.png \
    icons/folder.png \
    icons/edit.png \
    icons/developer.png \
    icons/delete.png \
    icons/chapters.png \
    icons/bookmarks.png \
    icons/bookmark.png \
    icons/book.png \
    icons/back.png \
    icons/add.png \
    icons/star.png \
    screenshots/ovi-screenshot-full-0.png \
    screenshots/ovi-screenshot-2.png \
    screenshots/ovi-screenshot-1.png \
    screenshots/ovi-screenshot-0.png \
    screenshots/2011-10-15_23-45-58.png \
    screenshots/2011-10-15_21-59-28.png \
    screenshots/2011-10-15_21-58-47.png \
    screenshots/2011-10-15_21-58-05.png \
    screenshots/2011-10-15_21-56-29.png \
    screenshots/2011-10-15_21-56-10.png \
    screenshots/2011-10-15_21-55-19.png \
    LICENSE.txt \
    README.markdown \
    books/2BR02B/mimetype \
    books/2BR02B/21279/www.gutenberg.org@files@21279@21279-h@21279-h-0.htm \
    books/2BR02B/21279/toc.ncx \
    books/2BR02B/21279/project.htm \
    books/2BR02B/21279/pgepub.css \
    books/2BR02B/21279/cover.jpg \
    books/2BR02B/21279/content.opf \
    books/2BR02B/21279/1.css \
    books/2BR02B/21279/0.css \
    books/2BR02B/META-INF/container.xml \
    books/Makefile \
    texts/about.html \
    texts/booksources.html

HEADERS += \
    backend/book.h \
    backend/bookdb.h \
    backend/bookfinder.h \
    backend/containerhandler.h \
    backend/xmlhandler.h \
    backend/opshandler.h \
    backend/extractzip.h \
    backend/unzip/unzip.h \
    backend/unzip/ioapi.h \
    backend/library.h \
    backend/ncxhandler.h \
    backend/xmlerrorhandler.h \
    backend/trace.h \
    backend/platform.h \
    backend/bookmark.h \
    backend/coverprovider.h \
    backend/contentitem.h \
    backend/eventfilter.h \
    backend/preferences.h \
    backend/splash.h \
    backend/bookdbworker.h

RESOURCES += \
    ionic.qrc

