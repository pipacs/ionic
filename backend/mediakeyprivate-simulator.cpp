#include <QKeyEvent>

#include "mediakey.h"
#include "mediakeyprivate.h"

#if defined(QT_SIMULATOR)

MediaKeyPrivate::MediaKeyPrivate(MediaKey *parent): QObject(parent), d_ptr(parent) {
}

MediaKeyPrivate::~MediaKeyPrivate() {
}

bool MediaKeyPrivate::eventFilter(QObject *obj, QEvent *event) {
    return QObject::eventFilter(obj, event);
}

#endif // QT_SIMULATOR
