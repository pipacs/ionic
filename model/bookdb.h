#ifndef BOOKDB_H
#define BOOKDB_H

#include <QObject>
#include <QVariantHash>
#include <QtSql>
#include <QStringList>

#include "bookdbworker.h"

/** Facade for storing/retrieving book meta-data. */
class BookDb {
public:
    static BookDb *instance();
    static void close();
    QVariantHash load(const QString &book);
    void save(const QString &book, const QVariantHash &data);
    void remove(const QString &book);
    void removeAll();
    QStringList books();
    BookDbWorker *worker();

protected:
    BookDb();
    ~BookDb();
    BookDbWorker *worker_;
};

#endif // BOOKDB_H
