#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVariant>

class QString;

/**
 * Shallow wrapper for QSettings.
 * Emits valueChanged signals when a setting value has changed.
 */
class Settings: public QObject {
    Q_OBJECT

public:
    static Settings *instance();
    static void close();
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    void apply();

signals:
    void valueChanged(const QString &key);

protected:
    Settings();
};

#endif // SETTINGS_H
