#include "sortedlibrary.h"
#include "library.h"
#include "book.h"
#include "trace.h"

SortedLibrary::SortedLibrary(QObject *parent):
        QSortFilterProxyModel(parent), mSortBy(SortByTitle)
{
    setSourceModel(Library::instance());
    setDynamicSortFilter(true);
    sort(0);
}

void SortedLibrary::setSortBy(SortBy key)
{
    TRACE;
    if (mSortBy != key) {
        mSortBy = key;
        invalidate();
        sort(0);
    }
}

SortedLibrary::SortBy SortedLibrary::sortBy()
{
    return mSortBy;
}

bool SortedLibrary::lessThan(const QModelIndex &left,
                             const QModelIndex &right) const
{
    Book *leftBook = Library::instance()->book(left);
    Book *rightBook = Library::instance()->book(right);
    int ret = 0;

    switch (mSortBy) {
    case SortByAuthor:
        ret = compareBy(SortByAuthor, leftBook, rightBook);
        if (ret == 0) {
            ret = compareBy(SortByTitle, leftBook, rightBook);
        }
        break;
    default:
        ret = compareBy(SortByTitle, leftBook, rightBook);
        if (ret == 0) {
            ret = compareBy(SortByAuthor, leftBook, rightBook);
        }
    }

    return ret < 0;
}

int SortedLibrary::compareBy(SortBy key, Book *left, Book *right) const
{
    Q_ASSERT(left);
    Q_ASSERT(right);
    switch (key) {
    case SortByAuthor:
        return QString::localeAwareCompare(left->creators().join(" "),
                                           right->creators().join(" "));
    default:
        return QString::localeAwareCompare(left->shortName(),
                                           right->shortName());
    }
}
