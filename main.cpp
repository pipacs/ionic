#include <QtGui/QApplication>
#include <QtDeclarative>

#include "qmlapplicationviewer.h"
#include "backend/trace.h"
#include "backend/preferences.h"
#include "backend/contentitem.h"
#include "backend/book.h"
#include "backend/bookmark.h"
#include "backend/bookdb.h"
#include "backend/library.h"
#include "backend/coverprovider.h"
#include "backend/bookfinder.h"
#include "backend/platform.h"
#include "backend/mediakey.h"

int main(int argc, char *argv[]) {
    // Set up application
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    app->setApplicationName("Ionic");
    app->setApplicationVersion(Platform::instance()->version());
    app->setOrganizationDomain("pipacs.com");
    app->setOrganizationName("pipacs.com");

    // Set up tracing
    Preferences *settings = Preferences::instance();
    Trace::level = (QtMsgType)settings->value("tracelevel", (int)QtDebugMsg).toInt();
    Trace::setFileName(settings->value("tracefilename").toString());
    qInstallMsgHandler(Trace::messageHandler);
    qDebug() << "Ionic version " << Platform::instance()->version();

    // Register QML types
    qmlRegisterType<Bookmark>("com.pipacs.ionic.Bookmark", 1, 0, "Bookmark");
    qmlRegisterType<Book>("com.pipacs.ionic.Book", 1, 0, "Book");
    qmlRegisterType<Library>("com.pipacs.ionic.Library", 1, 0, "Library");
    qmlRegisterType<Preferences>("com.pipacs.ionic.Preferences", 1, 0, "Preferences");

    // Do book database management in a separate thread
    QThread *bookDbWorkerThread = new QThread;
    BookDb::instance()->worker()->moveToThread(bookDbWorkerThread);
    bookDbWorkerThread->start(QThread::LowestPriority);

    // Initialize library, load book from command line, or the last book, or the default book
    Library *library = Library::instance();
    library->load();
    Book *current;
    if (argc > 1) {
        qDebug() << argv[1];
        current = library->find(argv[1]);
        if (!current) {
            current = library->add(argv[1]);
        }
        if (current) {
            library->setNowReading(current);
        }
    }
    current = library->nowReading();
    if (!current->isValid()) {
        if (!library->bookCount()) {
            library->add(":/books/2BR02B.epub");
        }
        library->setNowReading(library->book(0));
    }

    // Do book import in a separate thread
    BookFinder *bookFinder = new BookFinder;
    BookFinderWorker *bookFinderWorker = new BookFinderWorker;
    QThread *bookFinderWorkerThread = new QThread;
    bookFinderWorker->moveToThread(bookFinderWorkerThread);
    bookFinder->connect(bookFinder, SIGNAL(findRequested()), bookFinderWorker, SLOT(doFind()));
    bookFinderWorker->connect(bookFinderWorker, SIGNAL(begin(int)), bookFinder, SIGNAL(begin(int)));
    bookFinderWorker->connect(bookFinderWorker, SIGNAL(add(QString)), bookFinder, SIGNAL(add(QString)));
    bookFinderWorker->connect(bookFinderWorker, SIGNAL(done(int)), bookFinder, SIGNAL(done(int)));
    bookFinderWorkerThread->start(QThread::LowestPriority);

    // Set up and show QML widget with main.qml
    QmlApplicationViewer *viewer = new QmlApplicationViewer;
    MediaKey *mediaKey = new MediaKey(viewer);
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->engine()->addImageProvider(QString("covers"), new CoverProvider);
    viewer->rootContext()->setContextProperty("library", library);
    viewer->rootContext()->setContextProperty("prefs", settings);
    Book *emptyBook = new Book();
    viewer->rootContext()->setContextProperty("emptyBook", emptyBook);
    viewer->rootContext()->setContextProperty("bookFinder", bookFinder);
    viewer->rootContext()->setContextProperty("platform", Platform::instance());
    viewer->rootContext()->setContextProperty("mediaKey", mediaKey);
    viewer->setSource(QUrl("qrc:/qml/main.qml"));
    viewer->showExpanded();

#if defined(MEEGO_EDITION_HARMATTAN)
    // Install event filter to capture/release volume keys
    viewer->installEventFilter(mediaKey);
#endif

    // Run application
    int ret = app->exec();

    // Delete singletons
    delete viewer;
    bookFinderWorkerThread->quit();
    bookFinderWorkerThread->wait();
    delete bookFinderWorkerThread;
    delete bookFinderWorker;
    delete bookFinder;
    delete emptyBook;
    Library::close();
    bookDbWorkerThread->quit();
    bookDbWorkerThread->wait();
    delete bookDbWorkerThread;
    BookDb::close();
    Preferences::close();
    Platform::close();

    return ret;
}
