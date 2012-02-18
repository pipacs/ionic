#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QObject>
#include <qplatformdefs.h>
#if defined(MEEGO_EDITION_HARMATTAN)
#include <policy/resource-set.h>
#endif

#include "preferences.h"

class EventFilter: public QObject {
    Q_OBJECT
public:
    explicit EventFilter(QObject *parent = 0);
    ~EventFilter();
public slots:
    void onSettingChanged(const QString &key);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    bool captureVolumeKeys;
    bool active;
    Preferences *settings;
#if defined(MEEGO_EDITION_HARMATTAN)
    ResourcePolicy::ResourceSet *resourceSet;
#endif
};

#endif // EVENTFILTER_H
