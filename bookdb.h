#ifndef BOOKDB_H
#define BOOKDB_H

#include <QVariantHash>
#include <QtSql>

class QString;

/** Persistent storage for book meta-data. */
class BookDb {
public:
    static BookDb *instance();
    static void close();
    QVariantHash load(const QString &book);
    void save(const QString &book, const QVariantHash &data);
    void remove(const QString &book);
    void removeAll();
    QStringList books();

private:
    BookDb();
    ~BookDb();
    void create();
    QSqlDatabase db;
};

#endif // BOOKDB_H
