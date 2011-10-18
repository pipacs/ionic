#include "bookdb.h"
#include "platform.h"
#include "trace.h"

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

BookDb::BookDb(): QObject() {
    worker_ = new BookDbWorker();
}

BookDb::~BookDb() {
    delete worker_;
}

BookDbWorker *BookDb::worker() {
    return worker_;
}

QVariantHash BookDb::load(const QString &book) {
    QVariantHash ret;
    if (!QMetaObject::invokeMethod(
            worker_,
            "doLoad",
            Q_RETURN_ARG(QVariantHash, ret),
            Q_ARG(QString, book))) {
        qCritical() << "BookDb::load: Invoking remote loader failed";
    }
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
    if (!QMetaObject::invokeMethod(
            worker_,
            "doListBooks",
            Q_RETURN_ARG(QStringList, ret))) {
        qCritical() << "BookDb::books: Invoking remote loader failed";
    }
    return ret;
}

BookDbWorker::BookDbWorker() {
    TRACE;
    bool shouldCreate = false;
    QFileInfo info(Platform::instance()->dbPath());
    if (!info.exists()) {
        QDir dbDir;
        if (!dbDir.mkpath(info.absolutePath())) {
            qCritical() << "Could not create" << info.absolutePath();
        }
        shouldCreate = true;
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::toNativeSeparators(Platform::instance()->dbPath()));
    if (!db.open()) {
        qCritical() << "Could not open" << Platform::instance()->dbPath() << ": Error" << db.lastError().text();
    }
    if (shouldCreate) {
        create();
    }
}

BookDbWorker::~BookDbWorker() {
    TRACE;
    db.close();
}

void BookDbWorker::create() {
    TRACE;
    QSqlQuery query;
    if (!query.exec("create table book (name text primary key, content blob)")) {
        qCritical() << "Failed to create database:" << query.lastError().text();
    }
}

QVariantHash BookDbWorker::doLoad(const QString &book) {
    TRACE;
    qDebug() << book;
    QVariantHash ret;
    QByteArray bytes;
    QSqlQuery query("select content from book where name = ?");
    query.bindValue(0, book);
    query.setForwardOnly(true);
    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
        return ret;
    }
    while (query.next()) {
        bytes = query.value(0).toByteArray();
        QDataStream in(bytes);
        in >> ret;
        break;
    }
    return ret;
}

void BookDbWorker::doSave(const QString &book, const QVariantHash &data) {
    TRACE;
    qDebug() << book;
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    out << data;
    QSqlQuery query("insert or replace into book values (?, ?)");
    query.bindValue(0, book);
    query.bindValue(1, bytes);
    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
    }
}

void BookDbWorker::doRemove(const QString &book) {
    TRACE;
    qDebug() << book;
    QSqlQuery query("delete from book where name = ?");
    query.bindValue(0, book);
    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
    }
}

QStringList BookDbWorker::doListBooks() {
    TRACE;
    QStringList ret;
    QSqlQuery query("select name from book");
    query.setForwardOnly(true);
    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
        return ret;
    }
    while (query.next()) {
        ret.append(query.value(0).toString());
    }
    qDebug() << ret;
    return ret;
}

void BookDbWorker::doRemoveAll() {
    TRACE;
    foreach (QString book, doListBooks()) {
        doRemove(book);
    }
}
