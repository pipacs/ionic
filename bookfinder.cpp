#include <QFileInfoList>
#include <QStringList>
#include <QDir>
#include <QDesktopServices>

#include "model/library.h"
#include "bookfinder.h"
#include "trace.h"

BookFinderWorker::BookFinderWorker(QObject *parent): QObject(parent) {
}

void BookFinderWorker::doFind() {
    TRACE;
    QStringList booksFound;
    int toAdd = 0;
    int added = 0;

    // Gather all .epub files
    QStringList filters(QString("*.epub"));
    QStringList paths;
    paths.append(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    paths.append(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + QString("/Download"));
    paths.append(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + QString("/Books"));
    paths.append(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
    paths.append(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    foreach (QString path, paths) {
        qDebug() << "Checking" << path;
        QFileInfoList entries = QDir(path).entryInfoList(filters, QDir::Files | QDir::Readable);
        foreach (QFileInfo entry, entries) {
            booksFound.append(entry.absoluteFilePath());
        }
    }

    // Count the number of new books
    Library *library = Library::instance();
    foreach (QString path, booksFound) {
        if (library->find(path) == -1) {
            toAdd++;
        }
    }

    // Add new books to the library
    emit begin(toAdd);
    foreach (QString path, booksFound) {
        if (library->find(path) == -1) {
            qDebug() << "New book" << path;
            Book *book = library->add(path);
            emit add(book->title());
            added++;
        }
    }
    emit done(added);
}
