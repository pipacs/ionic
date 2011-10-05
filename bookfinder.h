#ifndef BOOKFINDER_H
#define BOOKFINDER_H

#include <QObject>

class QString;

/** Find new books in candidate folders, add them to the library. */
class BookFinder: public QObject {
    Q_OBJECT

public:
    explicit BookFinder(QObject *parent = 0);

public slots:
    /** Start finding books and adding them to the library. */
    Q_INVOKABLE void find();

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

#endif // BOOKFINDER_H
