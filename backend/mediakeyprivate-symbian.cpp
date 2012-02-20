#include "mediakey.h"
#include "mediakeyprivate.h"

#if defined(Q_OS_SYMBIAN)

#include <QDebug>
#include <w32std.h>
#include <apgwgnam.h>
#include <aknkeylock.h>
#include <coedef.h>

MediaKeyPrivate::MediaKeyPrivate(MediaKey *parent): d_ptr(parent) {
    QT_TRAP_THROWING(iInterfaceSelector = CRemConInterfaceSelector::NewL());
    QT_TRAP_THROWING(iCoreTarget = CRemConCoreApiTarget::NewL(*iInterfaceSelector, *this));
    iInterfaceSelector->OpenTargetL();
}

MediaKeyPrivate::~MediaKeyPrivate() {
    delete iInterfaceSelector;
    delete iCoreTarget;
}

// Callback when media keys are pressed
void MediaKeyPrivate::MrccatoCommand(TRemConCoreApiOperationId operation, TRemConCoreApiButtonAction) {
    bool locked = false;
    RAknKeylock2 keyLock;
    TInt err;

    // Ignore key if key lock is on
    TRAP(err, keyLock.Connect());
    if (err == KErrNone) {
        locked = (bool)keyLock.IsKeyLockEnabled();
        keyLock.Close();
    } else {
        qWarning() << "MediaKeyPrivate::MrccatoCommand: IsKeyLockEnabled: Error %d" << (int)err;
    }
    if (locked) {
        return;
    }

    switch (operation) {
    case ERemConCoreApiVolumeUp:
        emit d_ptr->volumeUpPressed();
        break;
    case ERemConCoreApiVolumeDown:
        emit d_ptr->volumeDownPressed();
        break;
    default:
        break;
    }
}

bool MediaKeyPrivate::eventFilter(QObject *obj, QEvent *event) {
    return QObject::eventFilter(obj, event);
}

#endif // Q_OS_SYMBIAN

