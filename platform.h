#ifndef PLATFORM_H
#define PLATFORM_H

#include <QObject>
#include <QString>
#include <qmsystem2/qmdisplaystate.h>

class QWidget;

/** Platform abstractions. */
class Platform: public QObject {
    Q_OBJECT
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(QString version READ version CONSTANT)

public:
    static Platform *instance();
    static void close();

    int brightness();
    void setBrightness(int value);
    QString dbPath();
    void restart(char *argv[]);
    QString version();
    QString downloadDir();
    void information(const QString &label, QWidget *parent = 0);
    QString traceFileName();
    Q_INVOKABLE void pauseBlanking();

signals:
    void brightnessChanged();

protected:
    Platform();
    ~Platform();
    MeeGo::QmDisplayState *displayState;
};

#endif // PLATFORM_H
