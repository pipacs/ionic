#ifndef BOOKDB_H
#define BOOKDB_H

#include <QObject>
#include <QVariantHash>
#include <QtSql>
#include <QMutex>

class QString;

/** Do the real work of storing/retrieving book meta-data. */
class BookDbWorker: public QObject {
    Q_OBJECT

public:
    BookDbWorker();
    ~BookDbWorker();

public slots:
    QVariantHash doLoad(const QString &book);
    void doSave(const QString &book, const QVariantHash &data);
    void doRemove(const QString &book);
    void doRemoveAll();
    QStringList doListBooks();

private:
    void create();
    QSqlDatabase db;
    mutable QMutex mutex;
};

/** Facade for storing/retrieving book meta-data. */
class BookDb: public QObject {
    Q_OBJECT

public:
    static BookDb *instance();
    static void close();
    QVariantHash load(const QString &book);
    void save(const QString &book, const QVariantHash &data);
    void remove(const QString &book);
    void removeAll();
    QStringList books();
    BookDbWorker *worker();

private:
    BookDb();
    ~BookDb();
    BookDbWorker *worker_;
};

/** BookDb worker thread. */
class BookDbWorkerThread: public QThread {
public:
    void run() {exec();}
};

#endif // BOOKDB_H
