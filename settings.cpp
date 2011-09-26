#include <QSettings>
#include <QString>
#include <QStringList>

#include "settings.h"
#include "trace.h"

static Settings *theInstance;

Settings::Settings(): QObject(0)
{
}

Settings *Settings::instance()
{
    if (!theInstance) {
        theInstance = new Settings();
    }
    return theInstance;
}

void Settings::close()
{
    delete theInstance;
    theInstance = 0;
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    QSettings s;
    s.setValue(QString("settings/") + key, value);
    emit valueChanged(key);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
    QSettings s;
    return s.value(QString("settings/") + key, defaultValue);
}

void Settings::apply()
{
    TRACE;
    QSettings s;
    foreach (QString key, s.allKeys()) {
        if (key.startsWith("settings/")) {
            key = key.mid(9);
            setValue(key, value(key));
        }
    }
}
