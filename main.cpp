#include <QtGui/QApplication>
#include <QtDeclarative>

#include "qmlapplicationviewer.h"
#include "trace.h"
#include "preferences.h"
#include "model/contentitem.h"
#include "model/book.h"
#include "model/bookmark.h"
#include "model/bookdb.h"
#include "model/library.h"
#include "model/coverprovider.h"
#include "bookfinder.h"
#include "eventfilter.h"
#include "splash.h"
#include "platform.h"

int main(int argc, char *argv[]) {
    // Set up application
    QApplication app(argc, argv);
    app.setApplicationName("Ionic");
    app.setApplicationVersion(Platform::instance()->version());
    app.setOrganizationDomain("pipacs.com");
    app.setOrganizationName("pipacs.com");

    // Set up tracing
    Preferences *settings = Preferences::instance();
    Trace::level = (QtMsgType)settings->value("tracelevel", (int)QtDebugMsg).toInt();
    Trace::setFileName(settings->value("tracefilename").toString());
    qInstallMsgHandler(Trace::messageHandler);
    qDebug() << "Ionic version " << Platform::instance()->version();

    // Show splash screen
    Splash splash;
    splash.show();
    app.processEvents();

    // Register QML types
    qmlRegisterType<Bookmark>("com.pipacs.ionic.Bookmark", 1, 0, "Bookmark");
    qmlRegisterType<Book>("com.pipacs.ionic.Book", 1, 0, "Book");
    qmlRegisterType<Library>("com.pipacs.ionic.Library", 1, 0, "Library");
    qmlRegisterType<Preferences>("com.pipacs.ionic.Preferences", 1, 0, "Preferences");

    // Initialize library, load last book or default book
    Library *library = Library::instance();
    library->load();
    Book *current = library->nowReading();
    if (!current->isValid()) {
        if (!library->bookCount()) {
            library->add(":/books/2BR02B.epub");
        }
        library->setNowReading(library->book(0));
    }

    // Set up and show QML widget with main.qml
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.engine()->addImageProvider(QString("covers"), new CoverProvider);
    viewer.rootContext()->setContextProperty("library", library);
    viewer.rootContext()->setContextProperty("prefs", settings);
    Book *emptyBook = new Book();
    viewer.rootContext()->setContextProperty("emptyBook", emptyBook);
    BookFinder *bookFinder = new BookFinder();
    BookFinderWorker *bookFinderWorker = new BookFinderWorker();
    BookFinderWorkerThread *bookFinderWorkerThread = new BookFinderWorkerThread();
    bookFinderWorker->moveToThread(bookFinderWorkerThread);
    bookFinder->connect(bookFinder, SIGNAL(findRequested()), bookFinderWorker, SLOT(doFind()));
    bookFinderWorker->connect(bookFinderWorker, SIGNAL(begin(int)), bookFinder, SIGNAL(begin(int)));
    bookFinderWorker->connect(bookFinderWorker, SIGNAL(add(QString)), bookFinder, SIGNAL(add(QString)));
    bookFinderWorker->connect(bookFinderWorker, SIGNAL(done(int)), bookFinder, SIGNAL(done(int)));
    bookFinderWorkerThread->start();
    bookFinderWorkerThread->setPriority(QThread::LowestPriority);
    viewer.rootContext()->setContextProperty("platform", Platform::instance());
    viewer.setMainQmlFile(QLatin1String("qml/ionic/main.qml"));
    // viewer.setFocusPolicy(Qt::StrongFocus);
    // viewer.setFocus();
    // viewer.activateWindow();
    viewer.showExpanded();

    // Install event filter to capture/release volume keys
    EventFilter *eventFilter = new EventFilter(&viewer);
    viewer.installEventFilter(eventFilter);

    // Close splash screen, start application
    splash.close();
    int ret = app.exec();

    // Delete singletons
    bookFinderWorkerThread->quit();
    bookFinderWorkerThread->wait();
    delete bookFinderWorkerThread;
    delete bookFinderWorker;
    delete bookFinder;
    delete emptyBook;
    Library::close();
    BookDb::close();
    Preferences::close();

    return ret;
}
