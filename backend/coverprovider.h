#ifndef COVERPROVIDER_H
#define COVERPROVIDER_H

#include <QDeclarativeImageProvider>

class CoverProvider: public QDeclarativeImageProvider {
public:
    CoverProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // COVERPROVIDER_H
