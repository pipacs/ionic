#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QObject>
#include <policy/resource-set.h>
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
    ResourcePolicy::ResourceSet *resourceSet;
    Preferences *settings;
};

#endif // EVENTFILTER_H
