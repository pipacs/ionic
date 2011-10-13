#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVariant>
#include "trace.h"

class QString;

/**
 * Shallow wrapper for QSettings.
 * Emits valueChanged signals when a setting value has changed, provides some settings as QML properties.
 */
class Settings: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool useVolumeKeys READ useVolumeKeys WRITE setUseVolumeKeys NOTIFY valueChanged)
    Q_PROPERTY(bool useSwipe READ useSwipe WRITE setUseSwipe NOTIFY valueChanged)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY valueChanged)
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY valueChanged)

public:
    static Settings *instance();
    static void close();
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    void apply();

    bool useVolumeKeys() {return value("usevolumekeys").toBool();}
    void setUseVolumeKeys(bool v) {setValue("usevolumekeys", v);}
    bool useSwipe() {return value("useswipe").toBool();}
    void setUseSwipe(bool v) {setValue("useswipe", v);}
    qreal zoom() {return value("zoom", 100).toReal();}
    void setZoom(qreal v) {setValue("zoom", v);}
    QString style() {return value("style", "day").toString();}
    void setStyle(const QString &v) {setValue("style", v);}

signals:
    void valueChanged(const QString &key);

protected:
    Settings();
};

#endif // SETTINGS_H
