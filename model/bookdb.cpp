#include "bookdb.h"
#include "bookdbworker.h"
#include "platform.h"

BookDb *theInstance;

BookDb *BookDb::instance() {
    if (!theInstance) {
        theInstance = new BookDb;
    }
    return theInstance;
}

void BookDb::close() {
    if (theInstance) {
        delete theInstance;
        theInstance = 0;
    }
}

BookDb::BookDb() {
    worker_ = new BookDbWorker;
}

BookDb::~BookDb() {
    delete worker_;
}

BookDbWorker *BookDb::worker() {
    return worker_;
}

QVariantHash BookDb::load(const QString &book) {
    QVariantHash ret;

#if 0
    // FIXME: This will only work in Qt 4.8: https://bugreports.qt.nokia.com/browse/QTBUG-10440
    if (!QMetaObject::invokeMethod(
            worker_,
            "doLoad",
            Q_RETURN_ARG(QVariantHash, ret),
            Q_ARG(QString, book))) {
        qCritical() << "BookDb::load: Invoking remote loader failed";
    }
#else
    ret = worker_->doLoad(book);
#endif
    return ret;
}

void BookDb::save(const QString &book, const QVariantHash &data) {
    if (!QMetaObject::invokeMethod(
            worker_,
            "doSave",
            Q_ARG(QString, book),
            Q_ARG(QVariantHash, data))) {
        qCritical() << "BookDb::save: Invoking remote loader failed";
    }
}

void BookDb::remove(const QString &book) {
    if (!QMetaObject::invokeMethod(
            worker_,
            "doRemove",
            Q_ARG(QString, book))) {
        qCritical() << "BookDb::remove: Invoking remote loader failed";
    }
}

void BookDb::removeAll() {
    if (!QMetaObject::invokeMethod(
            worker_,
            "doRemoveAll")) {
        qCritical() << "BookDb::removeAll: Invoking remote loader failed";
    }
}

QStringList BookDb::books() {
    QStringList ret;
#if 0
    // FIXME: This will only work in Qt 4.8: https://bugreports.qt.nokia.com/browse/QTBUG-10440
    if (!QMetaObject::invokeMethod(
            worker_,
            "doListBooks",
            Q_RETURN_ARG(QStringList, ret))) {
        qCritical() << "BookDb::books: Invoking remote loader failed";
    }
#else
    ret = worker_->doListBooks();
#endif
    return ret;
}
