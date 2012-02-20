#include <QDebug>

#include "mediakey.h"
#include "mediakeyprivate.h"

MediaKey::MediaKey(QObject *parent): QObject(parent) {
    d_ptr = new MediaKeyPrivate(this);
}

MediaKey::~MediaKey() {
    qDebug() << "MediaKey::~MediaKey";
}

bool MediaKey::eventFilter(QObject *obj, QEvent *event) {
    return d_ptr->eventFilter(obj, event);
}
