#ifndef PLATFORM_H
#define PLATFORM_H

#include <QObject>
#include <QString>
#include <qmsystem2/qmdisplaystate.h>

class QWidget;

/// Platform abstractions.
class Platform: public QObject {
    Q_OBJECT
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(QString version READ version CONSTANT)

    /// Operating system name, like "harmattan"
    Q_PROPERTY(QString osName READ osName CONSTANT)

    /// Operating system version
    Q_PROPERTY(QString osVersion READ osVersion CONSTANT)

public:
    static Platform *instance();
    static void close();

    int brightness();
    void setBrightness(int value);
    QString dbPath();
    QString version();
    QString downloadDir();
    QString traceFileName();
    Q_INVOKABLE void pauseBlanking();
    Q_INVOKABLE QString text(const QString &key);
    Q_INVOKABLE void browse(const QString &url);

    /// Operating system name.
    QString osName();

    /// Operating system version
    QString osVersion();

signals:
    void brightnessChanged();

protected:
    Platform();
    ~Platform();
    MeeGo::QmDisplayState *displayState;
};

#endif // PLATFORM_H
