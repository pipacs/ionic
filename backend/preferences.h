#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QVariant>
#include <QDebug>

#include "trace.h"

class QString;

/**
 * Shallow wrapper for QSettings.
 * Emits signals when a setting has changed, provides some settings as QML properties.
 */
class Preferences: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool useVolumeKeys READ useVolumeKeys WRITE setUseVolumeKeys NOTIFY useVolumeKeysChanged)
    Q_PROPERTY(bool useSwipe READ useSwipe WRITE setUseSwipe NOTIFY useSwipeChanged)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(bool preventBlanking READ preventBlanking WRITE setPreventBlanking NOTIFY preventBlankingChanged)
    Q_PROPERTY(bool showToolBar READ showToolBar WRITE setShowToolBar NOTIFY showToolBarChanged)
    Q_PROPERTY(QString font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(bool usePressAndHold READ usePressAndHold WRITE setUsePressAndHold NOTIFY usePressAndHoldChanged)
    Q_PROPERTY(bool useTap READ useTap WRITE setUseTap NOTIFY useTapChanged)
    Q_PROPERTY(int margin READ margin WRITE setMargin NOTIFY marginChanged)

public:
    static Preferences *instance();
    static void close();
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    void apply();

    bool useVolumeKeys() {return value("usevolumekeys").toBool();}
    void setUseVolumeKeys(bool v) {setValue("usevolumekeys", v); emit useVolumeKeysChanged();}
    bool useSwipe() {return value("useswipe", true).toBool();}
    void setUseSwipe(bool v) {setValue("useswipe", v); emit useSwipeChanged();}
    qreal zoom() {return value("zoom", 100).toReal();}
    void setZoom(qreal v) {setValue("zoom", v); emit zoomChanged();}
    QString style() {return value("style", "day").toString();}
    void setStyle(const QString &v) {setValue("style", v); emit styleChanged();}
    int orientation() {return value("orientation", 0).toInt();}
    void setOrientation(int v) {setValue("orientation", v); emit orientationChanged();}
    bool preventBlanking() {return value("preventblanking", false).toBool();}
    void setPreventBlanking(bool v) {setValue("preventblanking", v); emit preventBlankingChanged();}
    bool showToolBar() {return value("showtoolbar", true).toBool();}
    void setShowToolBar(bool v) {setValue("showtoolbar", v); emit showToolBarChanged();}
    QString font() {return value("font", "Times New Roman").toString();}
    void setFont(const QString &v) {setValue("font", v); emit fontChanged();}
    bool usePressAndHold() {return value("usepressandhold").toBool();}
    void setUsePressAndHold(bool v) {setValue("usepressandhold", v); emit usePressAndHoldChanged();}
    bool useTap() {return value("usetap").toBool();}
    void setUseTap(bool v) {setValue("usetap", v); emit useTapChanged();}
    int margin() {return value("margin", 10).toInt();}
    void setMargin(int v) {setValue("margin", v); emit marginChanged();}

signals:
    void valueChanged(const QString &key);
    void useVolumeKeysChanged();
    void useSwipeChanged();
    void zoomChanged();
    void styleChanged();
    void orientationChanged();
    void preventBlankingChanged();
    void showToolBarChanged();
    void fontChanged();
    void usePressAndHoldChanged();
    void useTapChanged();
    void marginChanged();

protected:
    Preferences();
};

#endif // PREFERENCES_H
