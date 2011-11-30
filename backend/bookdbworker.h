#ifndef BOOKDBWORKER_H
#define BOOKDBWORKER_H

#include <QObject>
#include <QVariantHash>
#include <QString>
#include <QStringList>
#include <QMutex>
#include <QThread>
#include <QSqlDatabase>

/** Do the real work of storing/retrieving book meta-data. */
class BookDbWorker: public QObject {
    Q_OBJECT

public:
    BookDbWorker();
    virtual ~BookDbWorker();

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

#endif // BOOKDBWORKER_H
