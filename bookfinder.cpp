#include <QFileInfoList>
#include <QStringList>
#include <QDir>

#include "bookfinder.h"
#include "trace.h"

BookFinder::BookFinder(QObject *parent): QObject(parent) {
}

void BookFinder::find(const QString &path, const QStringList &books) {
    TRACE;
    QStringList booksFound;
    int toAdd = 0;
    int added = 0;

    QStringList filters(QString("*.epub"));
    QFileInfoList entries =
            QDir(path).entryInfoList(filters, QDir::Files | QDir::Readable);
    foreach (QFileInfo entry, entries) {
        booksFound.append(entry.absoluteFilePath());
    }

    foreach (QString found, booksFound) {
        if (!books.contains(found)) {
            toAdd++;
        }
    }
    emit begin(toAdd);
    foreach (QString found, booksFound) {
        if (!books.contains(found)) {
            qDebug() << "New book" << found;
            emit add(found);
            added++;
        }
    }
    emit done(added);
}
