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

Library::Library(QObject *parent): QObject(parent), mNowReading(noBook()), mSortBy(SortByTitle) {
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

    foreach(QString path, books) {
        Book *book = new Book(path);
        mBooks.append(book);
    }

    QSettings settings;
    QString currentPath = settings.value("lib/nowreading").toString();
    Book *found = find(currentPath);
    mNowReading = found? found: noBook();
    mSortBy = (SortBy)settings.value("lib/sortby", QVariant((int)SortByTitle)).toInt();
    sort();
    emit nowReadingChanged();
}

void Library::save() {
    TRACE;
    QSettings settings;
    settings.setValue("lib/nowreading", mNowReading->path());
    settings.setValue("lib/sortby", (int)mSortBy);
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
    sort();
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

QDeclarativeListProperty<Book> Library::books() {
    TRACE;
    return QDeclarativeListProperty<Book>(this, mBooks);
}

int Library::bookCount() {
    return mBooks.count();
}

void Library::setSortBy(SortBy order) {
    if (mSortBy != order) {
        mSortBy = order;
        emit sortByChanged();
        sort();
        save();
    }
}

Library::SortBy Library::sortBy() const {
    return mSortBy;
}

void Library::sort() {
    switch (mSortBy) {
    case SortByAuthor: qSort(mBooks.begin(), mBooks.end(), lessThanByAuthor); break;
    case SortByDateAdded: qSort(mBooks.begin(), mBooks.end(), lessThanByDateAdded); break;
    case SortByDateOpened: qSort(mBooks.begin(), mBooks.end(), lessThanByDateOpened); break;
    default: qSort(mBooks.begin(), mBooks.end(), lessThanByTitle);
    }
    emit booksChanged();
}

bool Library::lessThanByTitle(const Book *cb1, const Book *cb2) {
    Book *b1 = const_cast<Book *>(cb1);
    Book *b2 = const_cast<Book *>(cb2);
    return b1->title() < b2->title();
}

bool Library::lessThanByAuthor(const Book *cb1, const Book *cb2) {
    Book *b1 = const_cast<Book *>(cb1);
    Book *b2 = const_cast<Book *>(cb2);
    QString author1 = b1->creators().join(", ");
    QString author2 = b2->creators().join(", ");
    if (author1 == author2) {
        return b1->title() < b2->title();
    } else {
        return author1 < author2;
    }
}

bool Library::lessThanByDateAdded(const Book *cb1, const Book *cb2) {
    Book *b1 = const_cast<Book *>(cb1);
    Book *b2 = const_cast<Book *>(cb2);
    if (b1->dateAdded() == b2->dateAdded()) {
        return b1->title() < b2->title();
    } else {
        return b1->dateAdded() < b2->dateAdded();
    }
}

bool Library::lessThanByDateOpened(const Book *cb1, const Book *cb2) {
    Book *b1 = const_cast<Book *>(cb1);
    Book *b2 = const_cast<Book *>(cb2);
    if (b1->dateOpened() == b2->dateOpened()) {
        return b1->title() < b2->title();
    } else {
        return b1->dateOpened() < b2->dateOpened();
    }
}
