#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QVariant>
#include "trace.h"

class QString;

/**
 * Shallow wrapper for QSettings.
 * Emits valueChanged signals when a setting value has changed, provides some settings as QML properties.
 */
class Preferences: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool useVolumeKeys READ useVolumeKeys WRITE setUseVolumeKeys NOTIFY valueChanged)
    Q_PROPERTY(bool useSwipe READ useSwipe WRITE setUseSwipe NOTIFY valueChanged)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY valueChanged)
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY valueChanged)
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY valueChanged)
    Q_PROPERTY(bool preventBlanking READ preventBlanking WRITE setPreventBlanking NOTIFY valueChanged)
    Q_PROPERTY(bool showToolBar READ showToolBar WRITE setShowToolBar NOTIFY valueChanged)
    Q_PROPERTY(QString font READ font WRITE setFont NOTIFY valueChanged)

public:
    static Preferences *instance();
    static void close();
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    void apply();

    bool useVolumeKeys() {return value("usevolumekeys").toBool();}
    void setUseVolumeKeys(bool v) {setValue("usevolumekeys", v);}
    bool useSwipe() {return value("useswipe", true).toBool();}
    void setUseSwipe(bool v) {setValue("useswipe", v);}
    qreal zoom() {return value("zoom", 100).toReal();}
    void setZoom(qreal v) {setValue("zoom", v);}
    QString style() {return value("style", "day").toString();}
    void setStyle(const QString &v) {setValue("style", v);}
    int orientation() {return value("orientation", 0).toInt();}
    void setOrientation(int v) {setValue("orientation", v);}
    bool preventBlanking() {return value("preventblanking", false).toBool();}
    void setPreventBlanking(bool v) {setValue("preventblanking", v);}
    bool showToolBar() {return value("showtoolbar", true).toBool();}
    void setShowToolBar(bool v) {setValue("showtoolbar", v);}
    QString font() {return value("font", "Times New Roman").toString();}
    void setFont(const QString &v) {setValue("font", v);}

signals:
    void valueChanged(const QString &key);

protected:
    Preferences();
};

#endif // PREFERENCES_H
