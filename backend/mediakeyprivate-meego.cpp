#include <QKeyEvent>
#include <QDebug>

#include "mediakey.h"
#include "mediakeyprivate.h"

#if defined(MEEGO_EDITION_HARMATTAN)

MediaKeyPrivate::MediaKeyPrivate(MediaKey *parent): QObject(parent), d_ptr(parent), active(false) {
    resourceSet = new ResourcePolicy::ResourceSet("player");
    resourceSet->addResourceObject(new ResourcePolicy::ScaleButtonResource);
}

MediaKeyPrivate::~MediaKeyPrivate() {
    qDebug() << "MediaKeyPrivate::~MediaKeyPrivate";
    resourceSet->release();
    resourceSet->deleteResource(ResourcePolicy::ScaleButtonType);
    delete resourceSet;
}

bool MediaKeyPrivate::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::ApplicationDeactivate) {
        active = false;
        resourceSet->release();
    } else if (event->type() == QEvent::ApplicationActivate) {
        active = true;
        resourceSet->acquire();
    } else if (event->type() == QEvent::ActivationChange) {
        qDebug() << "MediaKey::eventFilter: ActivationChange";
        if (active) {
            active = false;
            resourceSet->release();
        } else {
            active = true;
            resourceSet->acquire();
        }
        // emit d_ptr->activate(active);
    } else if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_VolumeUp) {
            qDebug() << "MediaKeyPrivate::eventFilter: Volume up";
            emit d_ptr->volumeUpPressed();
            return true;
        } else if (keyEvent->key() == Qt::Key_VolumeDown) {
            qDebug() << "MediaKeyPrivate::eventFilter: Volume down";
            emit d_ptr->volumeDownPressed();
            return true;
        }
    }

    return QObject::eventFilter(obj, event);
}

#endif
