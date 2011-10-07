#include "library.h"
#include "book.h"
#include "trace.h"
#include "bookdb.h"

static Book *noBook() {
    static Book *instance;
    if (!instance) {
        instance = new Book();
    }
    return instance;
}

static Library *theInstance = 0;

Library::Library(QObject *parent): QObject(parent), mNowReading(noBook()) {
}

Library::~Library() {
    clear();
}

Library *Library::instance() {
    if (!theInstance) {
        theInstance = new Library();
    }
    return theInstance;
}

Book *Library::book(int index) {
    if ((index >= 0) && (index < mBooks.size())) {
        return mBooks[index];
    }
    qCritical() << "Library::book: Bad or invalid index" << index;
    return noBook();
}

void Library::close() {
    delete theInstance;
    theInstance = 0;
}

void Library::load() {
    TRACE;
    clear();
    QStringList books = BookDb::instance()->books();
    emit beginLoad(books.size());

    foreach(QString path, books) {
        emit loading(path);
        Book *book = new Book(path);
        connect(book, SIGNAL(opened(const QString &)), this, SLOT(onBookOpened(const QString &)));
        // book->load();
        mBooks.append(book);
    }

    QSettings settings;
    QString currentPath = settings.value("lib/nowreading").toString();
    Book *found = find(currentPath);
    mNowReading = found? found: noBook();
    emit endLoad();
    emit booksChanged();
    emit nowReadingChanged();
}

void Library::save() {
    TRACE;
    QSettings settings;
    settings.setValue("lib/nowreading", mNowReading->path());
}

Book *Library::add(const QString &path) {
    TRACE;
    if (path == "") {
        qCritical() << "Library::add: Empty path";
        return noBook();
    }
    if (find(path)) {
        qDebug() << "Book already exists in library";
        return noBook();
    }
    Book *book = new Book(path);
    book->peek();
    book->setDateAdded(QDateTime::currentDateTime().toUTC());
    mBooks.append(book);
    save();
    emit booksChanged();
    return book;
}

void Library::remove(Book *book) {
    TRACE;
    if (!book) {
        return;
    }
    if (book == mNowReading) {
        mNowReading = noBook();
        emit nowReadingChanged();
    }
    book->remove();
    mBooks.removeOne(book);
    save();
    emit booksChanged();
    delete book;
}

Book *Library::nowReading() const {
    TRACE;
    return mNowReading;
}

void Library::setNowReading(Book *book) {
    TRACE;
    if (find(book)) {
        mNowReading = book;
    } else {
        mNowReading = noBook();
    }
    save();
    emit nowReadingChanged();
}

void Library::clear() {
    TRACE;
    for (int i = 0; i < mBooks.size(); i++) {
        delete mBooks[i];
    }
    mBooks.clear();
    mNowReading = noBook();
    emit booksChanged();
    emit nowReadingChanged();
}

Book *Library::find(QString path) const {
    TRACE;
    if (path != "") {
        QString absolutePath = QFileInfo(path).absoluteFilePath();
        for (int i = 0; i < mBooks.size(); i++) {
            if (absolutePath == mBooks[i]->path()) {
                return mBooks[i];
            }
        }
    }
    qDebug() << "Not found" << path;
    return 0;
}

Book *Library::find(Book *book) const {
    TRACE;
    if (book) {
        for (int i = 0; i < mBooks.size(); i++) {
            if (book == mBooks[i]) {
                return book;
            }
        }
    }
    return 0;
}

void Library::onBookOpened(const QString &path) {
    TRACE;
    (void)path;
    // FIXME: Not needed anymore?
}

QDeclarativeListProperty<Book> Library::books() {
    TRACE;
    return QDeclarativeListProperty<Book>(this, mBooks);
}

int Library::bookCount() {
    return mBooks.count();
}
