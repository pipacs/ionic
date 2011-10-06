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

public:
    static Library *instance();
    static void close();
    void load();
    void save();
    int find(QString path) const;
    int find(const Book *book) const;
    Q_INVOKABLE void setNowReading(Book *book);
    Book *nowReading() const;
    Book *book(int index);
    QDeclarativeListProperty<Book> books();
    int bookCount();

signals:
    void nowReadingChanged();
    void beginUpgrade(int total);
    void upgrading(const QString &book);
    void endUpgrade();
    void beginLoad(int total);
    void loading(const QString &book);
    void endLoad();
    void booksChanged();

public slots:
    Q_INVOKABLE Book *add(const QString &path);
    Q_INVOKABLE void remove(Book *book);
    void onBookOpened(const QString &path);

private:
    explicit Library(QObject *parent = 0);
    ~Library();
    void clear();
    QList<Book *> mBooks;
    Book *mNowReading;
};

#endif // LIBRARY_H
