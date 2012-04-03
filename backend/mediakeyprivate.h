#ifndef MEDIAKEYPRIVATE_H
#define MEDIAKEYPRIVATE_H

#include <qplatformdefs.h>

#if defined(MEEGO_EDITION_HARMATTAN)
#   include <policy/resource-set.h>
#elif defined(Q_OS_SYMBIAN)
#   include <remconcoreapitargetobserver.h>
#   include <remconcoreapitarget.h>
#   include <remconinterfaceselector.h>
#elif defined(QT_SIMULATOR)
#else
#   error MediaKeyPrivate not implemented on this platform
#endif

class MediaKey;

class MediaKeyPrivate: public QObject
#if defined(Q_OS_SYMBIAN)
    , public MRemConCoreApiTargetObserver
#endif
{
public:
    MediaKeyPrivate(MediaKey *parent);
    ~MediaKeyPrivate();
    bool eventFilter(QObject *obj, QEvent *event);

    MediaKey *d_ptr;

#if defined(MEEGO_EDITION_HARMATTAN)
    bool active;
    ResourcePolicy::ResourceSet *resourceSet;
#elif defined(Q_OS_SYMBIAN)
    virtual void MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct);
    CRemConInterfaceSelector *iInterfaceSelector;
    CRemConCoreApiTarget *iCoreTarget;
#endif
};

#endif // MEDIAKEYPRIVATE_H
