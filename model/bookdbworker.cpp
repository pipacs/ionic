#include <QFileInfo>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

#include "bookdbworker.h"
#include "trace.h"
#include "platform.h"

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
    QMutexLocker locker(&mutex);
    if (!query.exec("create table book (name text primary key, content blob)")) {
        qCritical() << "Failed to create database:" << query.lastError().text();
    }
}

QVariantHash BookDbWorker::doLoad(const QString &book) {
    TRACE;
    qDebug() << book;
    QMutexLocker locker(&mutex);
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
    QMutexLocker locker(&mutex);
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
    QMutexLocker locker(&mutex);
    QSqlQuery query("delete from book where name = ?");
    query.bindValue(0, book);
    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
    }
}

QStringList BookDbWorker::doListBooks() {
    TRACE;
    QMutexLocker locker(&mutex);
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
