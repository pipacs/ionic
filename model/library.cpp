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
    mNowReading = book(find(currentPath));
    emit endLoad();
    emit booksChanged();
    emit nowReadingChanged();
}

void Library::save() {
    TRACE;
    QSettings settings;
    settings.setValue("lib/nowreading", mNowReading->path());
}

bool Library::add(const QString &path) {
    TRACE;
    if (path == "") {
        qCritical() << "Library::add: Empty path";
        return false;
    }
    if (find(path) != -1) {
        qDebug() << "Book already exists in library";
        return false;
    }
    Book *book = new Book(path);
    book->peek();
    book->setDateAdded(QDateTime::currentDateTime().toUTC());
    mBooks.append(book);
    save();
    emit booksChanged();
    return true;
}

void Library::remove(int index) {
    TRACE;
    Book *toRemove = book(index);
    if (!toRemove) {
        return;
    }
    if (toRemove == mNowReading) {
        mNowReading = 0;
        emit nowReadingChanged();
    }
    toRemove->remove();
    mBooks.removeAt(index);
    save();
    emit booksChanged();
    delete toRemove;
}

void Library::remove(const QString &path) {
    TRACE;
    remove(find(path));
}

Book *Library::nowReading() const {
    TRACE;
    return mNowReading;
}

void Library::setNowReading(Book *book) {
    TRACE;
    if (find(book) != -1) {
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

int Library::find(QString path) const {
    TRACE;
    if (path != "") {
        QString absolutePath = QFileInfo(path).absoluteFilePath();
        for (int i = 0; i < mBooks.size(); i++) {
            if (absolutePath == mBooks[i]->path()) {
                return i;
            }
        }
    }
    qDebug() << "Not found" << path;
    return -1;
}

int Library::find(const Book *book) const {
    TRACE;
    if (book) {
        for (int i = 0; i < mBooks.size(); i++) {
            if (book == mBooks[i]) {
                return i;
            }
        }
    }
    return -1;
}

void Library::onBookOpened(const QString &path) {
    TRACE;
    (void)path;
    // FIXME: What is this supposed to do
}

QDeclarativeListProperty<Book> Library::books() {
    TRACE;
    return QDeclarativeListProperty<Book>(this, mBooks);
}

int Library::bookCount() {
    return mBooks.count();
}
