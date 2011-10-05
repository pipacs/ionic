#ifndef BOOKFINDER_H
#define BOOKFINDER_H

#include <QObject>
#include <QThread>

class QString;

/** Request finding books. */
class BookFinder: public QObject {
    Q_OBJECT

public:
    explicit BookFinder(QObject *parent = 0): QObject(parent) {}
    Q_INVOKABLE void find() {emit findRequested();}

signals:
    void findRequested();
    void begin(int count);
    void add(const QString &title);
    void done(int count);
};

/** Do the real work of finding books. */
class BookFinderWorker: public QObject {
    Q_OBJECT

public:
    explicit BookFinderWorker(QObject *parent = 0);

public slots:
    /** Do the real work. */
    void doFind();

signals:
    /**
    Emitted before adding books to the library starts.
    @param  count   Number of books about to be be added.
    */
    void begin(int count);

    /**
    Emitted after adding a new book to the library.
    @param  title   Title of the book added.
    */
    void add(const QString &title);

    /**
    Emitted after all new books have been added to the library.
    @param  count   Total number of books added.
    */
    void done(int count);
};

/** Book finder worker thread. */
class BookFinderWorkerThread: public QThread {
public:
    void run() {
        exec();
    }
};

#endif // BOOKFINDER_H
