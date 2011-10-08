#ifndef LIBRARY_H
#define LIBRARY_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDeclarativeListProperty>

#include "book.h"

/** Library of books. */
class Library: public QObject {
    Q_OBJECT
    Q_PROPERTY(Book *nowReading READ nowReading WRITE setNowReading NOTIFY nowReadingChanged)
    Q_PROPERTY(QDeclarativeListProperty<Book> books READ books NOTIFY booksChanged)
    Q_PROPERTY(SortBy sortBy READ sortBy WRITE setSortBy NOTIFY sortByChanged)

public:
    static Library *instance();
    static void close();
    explicit Library(QObject *parent = 0);
    ~Library();
    void load();
    void save();
    Book *find(QString path) const;
    Book *find(Book *book) const;
    Q_INVOKABLE void setNowReading(Book *book);
    Book *nowReading() const;
    Book *book(int index);
    QDeclarativeListProperty<Book> books();
    int bookCount();
    enum SortBy {
        SortByTitle,
        SortByAuthor,
        SortByDateAdded,
        SortByDateOpened
    };
    Q_ENUMS(SortBy)
    void setSortBy(SortBy order);
    SortBy sortBy() const;

signals:
    void nowReadingChanged();
    void booksChanged();
    void sortByChanged();

public slots:
    Q_INVOKABLE Book *add(const QString &path);
    Q_INVOKABLE void remove(Book *book);

private:
    void clear();
    void sort();
    bool lessThan(const Book *b1, const Book *b2);
    QList<Book *> mBooks;
    Book *mNowReading;
    SortBy mSortBy;
    static bool lessThanByTitle(const Book *cb1, const Book *cb2);
    static bool lessThanByAuthor(const Book *cb1, const Book *cb2);
    static bool lessThanByDateAdded(const Book *cb1, const Book *cb2);
    static bool lessThanByDateOpened(const Book *cb1, const Book *cb2);
};

#endif // LIBRARY_H
