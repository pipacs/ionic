#ifndef PLATFORM_H
#define PLATFORM_H

#include <QObject>
#include <QString>
#include <qplatformdefs.h>

#if defined(MEEGO_EDITION_HARMATTAN)
#include <qmsystem2/qmdisplaystate.h>
#endif

class QWidget;

/// Platform abstractions.
class Platform: public QObject {
    Q_OBJECT

    /// Display brightness.
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)

    /// Application version.
    Q_PROPERTY(QString version READ version CONSTANT)

    /// Operating system name, like "harmattan".
    Q_PROPERTY(QString osName READ osName CONSTANT)

    /// Operating system version.
    Q_PROPERTY(QString osVersion READ osVersion CONSTANT)

    /// Application data directory.
    Q_PROPERTY(QString dataDir READ dataDir CONSTANT)

public:
    static Platform *instance();
    static void close();

    int brightness();
    void setBrightness(int value);
    QString dbPath();
    QString version() const {return IONIC_VERSION;}
    QString downloadDir();
    QString traceFileName();
    Q_INVOKABLE void pauseBlanking();
    Q_INVOKABLE QString text(const QString &key);
    Q_INVOKABLE void browse(const QString &url);

    /// Operating system name.
    QString osName();

    /// Operating system version
    QString osVersion();

    /// Application data directory.
    QString dataDir() const {return IONIC_DATA_DIR;}

signals:
    void brightnessChanged();

protected:
    Platform();
    ~Platform();
#if defined(MEEGO_EDITION_HARMATTAN)
    MeeGo::QmDisplayState *displayState;
#endif
};

#endif // PLATFORM_H
