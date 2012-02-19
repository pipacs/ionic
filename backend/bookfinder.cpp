#include <QFileInfoList>
#include <QStringList>
#include <QDir>
#include <QDesktopServices>
#include <qplatformdefs.h>

#include "library.h"
#include "bookfinder.h"
#include "trace.h"

BookFinderWorker::BookFinderWorker(QObject *parent): QObject(parent) {
}

void BookFinderWorker::doFind() {
    TRACE;
    QStringList booksFound;
    int toAdd = 0;
    int added = 0;

    // Gather all .epub files from all possible locations
#if defined(Q_OS_SYMBIAN)
    doFindInDir("E:/", booksFound, 0);
    doFindInDir("F:/", booksFound, 0);
#else
    doFindInDir(QDesktopServices::storageLocation(QDesktopServices::HomeLocation), booksFound, 0);
#endif

    // Count the number of new books
    Library *library = Library::instance();
    foreach (QString path, booksFound) {
        if (!library->find(path)) {
            toAdd++;
        }
    }

    // Add new books to the library
    emit begin(toAdd);
    foreach (QString path, booksFound) {
        if (!library->find(path)) {
            qDebug() << "New book" << path;
            Book *book = library->add(path);
            emit add(book->title());
            added++;
        }
    }
    emit done(added);
}

void BookFinderWorker::doFindInDir(const QString &dir, QStringList &results, int depth) {
#if defined(Q_OS_SYMBIAN)
    const int maxDepth = 3;
#else
    const int maxDepth = 6;
#endif
    if (depth == maxDepth) {
        qWarning() << "BookFinderWorker::doFindInDir: Bailing out after" << maxDepth << "recursions";
        return;
    }
    qDebug() << "BookFinderWorker::doFindInDir:" << dir << depth;
    QStringList epubFilters(QString("*.epub"));
    foreach (QFileInfo entry, QDir(dir).entryInfoList(epubFilters, QDir::Files | QDir::Readable)) {
        results.append(entry.absoluteFilePath());
    }
    QStringList dirFilters(QString("*"));
    foreach (QFileInfo entry, QDir(dir).entryInfoList(dirFilters, QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot)) {
        doFindInDir(entry.absoluteFilePath(), results, depth + 1);
    }
}
