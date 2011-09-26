#include "bookdb.h"
#include "platform.h"
#include "trace.h"

BookDb *theInstance;

BookDb *BookDb::instance()
{
    if (!theInstance) {
        theInstance = new BookDb;
    }
    return theInstance;
}

void BookDb::close()
{
    if (theInstance) {
        delete theInstance;
        theInstance = 0;
    }
}

BookDb::BookDb()
{
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
        qCritical() << "Could not open" << Platform::instance()->dbPath()
                << ": Error" << db.lastError().text();
    }
    if (shouldCreate) {
        create();
    }
}

BookDb::~BookDb()
{
    db.close();
}

void BookDb::create()
{
    TRACE;
    QSqlQuery query;
    if (!query.exec("create table book "
                    "(name text primary key, content blob)")) {
        qCritical() << "Failed to create database:"
                << query.lastError().text();
    }
}

QVariantHash BookDb::load(const QString &book)
{
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

void BookDb::save(const QString &book, const QVariantHash &data)
{
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

void BookDb::remove(const QString &book)
{
    TRACE;
    qDebug() << book;
    QSqlQuery query("delete from book where name = ?");
    query.bindValue(0, book);
    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
    }
}

QStringList BookDb::books()
{
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

void BookDb::removeAll()
{
    foreach (QString book, books()) {
        remove(book);
    }
}
