#include <QtGui/QApplication>
#include <QtDeclarative>

#include "qmlapplicationviewer.h"
#include "trace.h"
#include "settings.h"
#include "model/book.h"
#include "model/bookmark.h"
#include "model/bookdb.h"
#include "model/library.h"
#include "model/sortedlibrary.h"

static const char *ionicVersion =
#include "pkg/version.txt"
;

int main(int argc, char *argv[]) {
    // Set up application
    QApplication app(argc, argv);

    // Set up tracing
    Settings *settings = Settings::instance();
    Trace::level = (QtMsgType)settings->value("tracelevel", (int)QtDebugMsg).toInt();
    Trace::setFileName(settings->value("tracefilename").toString());
    qInstallMsgHandler(Trace::messageHandler);
    qDebug() << "Ionic version " << ionicVersion;

    // Register QML types
    qmlRegisterType<Bookmark>("com.pipacs.ionic.Bookmark", 1, 0, "Bookmark");
    qmlRegisterType<Book>("com.pipacs.ionic.Book", 1, 0, "Book");
    // qmlRegisterType<Library>("com.pipacs.ionic.Library", 1, 0, "Library");

    // Show QML widget with main.qml
    QmlApplicationViewer viewer;
    Library *library = Library::instance();
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.rootContext()->setContextProperty("library", library);
    Library::instance()->load();
    viewer.rootContext()->setContextProperty("settings", Settings::instance());
    viewer.setMainQmlFile(QLatin1String("qml/ionic/main.qml"));
    viewer.showExpanded();

    // Initialize library, load last book or default book
    Book *current = library->nowReading();
    if (current->isValid()) {
        library->setNowReading(current);
    } else {
        if (!library->rowCount()) {
            library->add(":/books/2BR02B.epub");
        }
        SortedLibrary sorted;
        library->setNowReading(library->book(sorted.mapToSource(sorted.index(0, 0))));
    }

    int ret = app.exec();

    Library::close();
    BookDb::close();
    Settings::close();

    return ret;
}
