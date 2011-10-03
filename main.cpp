#include <QtGui/QApplication>
#include <QtDeclarative>
#include <policy/resource-set.h>

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

    // Initialize library, load last book or default book
    Library *library = Library::instance();
    library->load();
    Book *current = library->nowReading();
    if (!current->isValid()) {
        if (!library->rowCount()) {
#if 0
            library->add(":/books/2BR02B.epub");
#endif
            library->add(":/books/hacker-monthly-2.epub");
        }
        SortedLibrary sorted;
        library->setNowReading(library->book(sorted.mapToSource(sorted.index(0, 0))));
    }

    // Show QML widget with main.qml
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.rootContext()->setContextProperty("library", library);
    viewer.rootContext()->setContextProperty("settings", Settings::instance());
    Book *emptyBook = new Book();
    viewer.rootContext()->setContextProperty("emptyBook", emptyBook);
    viewer.setMainQmlFile(QLatin1String("qml/ionic/main.qml"));
    viewer.showExpanded();

    // Acquire volume keys
    ResourcePolicy::ResourceSet* mySet = new ResourcePolicy::ResourceSet("player");
    mySet->addResourceObject(new ResourcePolicy::ScaleButtonResource);
    mySet->acquire();
    // FIXME: Install event filter to release volume keys when main view is not in front

    int ret = app.exec();

    // Delete singletons
    delete emptyBook;
    Library::close();
    BookDb::close();
    Settings::close();

    return ret;
}
