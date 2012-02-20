#ifndef MEDIAKEY_H
#define MEDIAKEY_H

#include <QObject>
#include <qplatformdefs.h>

#if defined(MEEGO_EDITION_HARMATTAN)
#   include <policy/resource-set.h>
#elif defined(Q_WS_S60)
#   include <remconcoreapitargetobserver.h>
#   include <remconcoreapitarget.h>
#   include <remconinterfaceselector.h>
#endif

class MediaKeyPrivate;

class MediaKey: public QObject {
    Q_OBJECT
public:
    MediaKey(QObject *parent = 0);
    virtual ~MediaKey();

signals:
    void volumeDownPressed();
    void volumeUpPressed();
    void activate();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    MediaKeyPrivate *d_ptr;
    friend class MediaKeyPrivate;
};

#endif // MEDIAKEY_H
