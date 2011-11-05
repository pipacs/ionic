#include "coverprovider.h"
#include "library.h"
#include "book.h"
#include "trace.h"

CoverProvider::CoverProvider(): QDeclarativeImageProvider(QDeclarativeImageProvider::Image) {
}

QImage CoverProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    (void)requestedSize;
    QImage ret;
    Library *Library = Library::instance();
    Book *book = Library->find(id);
    if (book) {
        ret = book->cover();
        if (ret.isNull()) {
            qWarning() << "CoverProvider::requestImage: Cover image is null";
        }
    } else {
        qWarning() << "CoverProvider::requestImage: Book not found";
    }
    if (size) {
        *size = ret.size();
    }
    return ret;
}
