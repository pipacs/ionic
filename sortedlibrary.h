#ifndef SORTEDLIBRARY_H
#define SORTEDLIBRARY_H

#include <QSortFilterProxyModel>

class Book;

/** Sorted library model. */
class SortedLibrary: public QSortFilterProxyModel {
    Q_OBJECT

public:
    enum SortBy {SortByTitle, SortByAuthor};
    explicit SortedLibrary(QObject *parent = 0);
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    void setSortBy(SortBy key);
    SortBy sortBy();

signals:

public slots:

protected:
    int compareBy(SortBy key, Book *left, Book *right) const;

private:
    SortBy mSortBy;
};

#endif // SORTEDLIBRARY_H
