#include "coverprovider.h"
#include "library.h"
#include "book.h"
#include "trace.h"

CoverProvider::CoverProvider(): QDeclarativeImageProvider(QDeclarativeImageProvider::Image) {
}

QImage CoverProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    TRACE;
    qDebug() << "Requesting image for" << id;
    Library *Library = Library::instance();
    int index = Library->find(id);
    if (index < 0) {
        return QImage();
    }
    Book *book = Library->book(index);
    return book->cover();
}
