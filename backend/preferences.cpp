#include <QSettings>
#include <QString>
#include <QStringList>

#include "preferences.h"
#include "trace.h"

static Preferences *inst;

Preferences::Preferences(): QObject(0) {
}

void Preferences::setValue(const QString &key, const QVariant &value) {
    QSettings s;
    s.setValue(QString("settings/") + key, value);
    emit valueChanged(key);
}

QVariant Preferences::value(const QString &key, const QVariant &defaultValue) const {
    QSettings s;
    return s.value(QString("settings/") + key, defaultValue);
}

void Preferences::apply() {
    QSettings s;
    foreach (QString key, s.allKeys()) {
        if (key.startsWith("settings/")) {
            key = key.mid(9);
            setValue(key, value(key));
        }
    }
}

Preferences *Preferences::instance() {
    if (!inst) {
        inst = new Preferences;
    }
    return inst;
}

void Preferences::close() {
    delete inst;
    inst = 0;
}
