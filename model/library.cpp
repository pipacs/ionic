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

Library::Library(QObject *parent): QAbstractListModel(parent), mNowReading(noBook()) {
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

int Library::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    } else {
        return mBooks.size();
    }
}

QVariant Library::data(const QModelIndex &index, int role) const {
    QVariant ret;
    if (!index.isValid()) {
        return ret;
    }

    switch (role) {
    case Qt::DisplayRole:
        ret = mBooks[index.row()]->name();
        break;
    case Qt::DecorationRole:
        ret.setValue(mBooks[index.row()]->coverImage());
        break;
    default:
        ;
    }

    return ret;
}

Book *Library::book(const QModelIndex &index) {
    if (index.isValid() && (index.row() >= 0) && (index.row() < mBooks.size())) {
        return mBooks[index.row()];
    }
    qCritical() << "Library::book: Bad or invalid index" << index.row();
    return 0;
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
    if (find(path).isValid()) {
        qDebug() << "Book already exists in library";
        return false;
    }
    int size = mBooks.size();
    beginInsertRows(QModelIndex(), size, size);
    Book *book = new Book(path);
    book->peek();
    book->setDateAdded(QDateTime::currentDateTime().toUTC());
    mBooks.append(book);
    save();
    endInsertRows();
    return true;
}

void Library::remove(const QModelIndex &index) {
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
    int row = index.row();
    beginRemoveRows(QModelIndex(), row, row);
    mBooks.removeAt(row);
    save();
    endRemoveRows();
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
    if (find(book).isValid()) {
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
    emit nowReadingChanged();
}

QModelIndex Library::find(QString path) const {
    TRACE;
    if (path != "") {
        QString absolutePath = QFileInfo(path).absoluteFilePath();
        for (int i = 0; i < mBooks.size(); i++) {
            if (absolutePath == mBooks[i]->path()) {
                return index(i);
            }
        }
    }
    return QModelIndex();
}

QModelIndex Library::find(const Book *book) const {
    TRACE;
    if (book) {
        for (int i = 0; i < mBooks.size(); i++) {
            if (book == mBooks[i]) {
                return index(i);
            }
        }
    }
    return QModelIndex();
}

void Library::onBookOpened(const QString &path) {
    TRACE;
    QModelIndex index = find(path);
    if (index.isValid()) {
        emit dataChanged(index, index);
    }
}

QStringList Library::bookPaths() {
    QStringList ret;
    foreach (Book *book, mBooks) {
        ret.append(book->path());
    }
    return ret;
}
