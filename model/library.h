#ifndef LIBRARY_H
#define LIBRARY_H

#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include <QString>
#include <QList>
#include <QStringList>

#include "book.h"

/** Library of books. */
class Library: public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(Book *nowReading READ nowReading WRITE setNowReading NOTIFY nowReadingChanged)

public:
    static Library *instance();
    static void close();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void load();
    void save();
    QModelIndex find(QString path) const;
    QModelIndex find(const Book *book) const;
    void setNowReading(Book *book);
    Book *nowReading() const;
    Book *book(const QModelIndex &index);
    QStringList bookPaths();

signals:
    void nowReadingChanged();
    void beginUpgrade(int total);
    void upgrading(const QString &book);
    void endUpgrade();
    void beginLoad(int total);
    void loading(const QString &book);
    void endLoad();

public slots:
    bool add(const QString &path);
    void remove(const QString &path);
    void remove(const QModelIndex &index);
    void onBookOpened(const QString &path);

private:
    explicit Library(QObject *parent = 0);
    ~Library();
    void clear();
    QList<Book *> mBooks;
    Book *mNowReading;
};

#endif // LIBRARY_H
