#ifndef PLATFORM_H
#define PLATFORM_H

#include <QObject>
#include <QString>
#include <QSize>

class QWidget;

/** Platform abstractions. */
class Platform: public QObject {
    Q_OBJECT
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(bool blanking READ blanking WRITE setBlanking NOTIFY blankingChanged)
    Q_PROPERTY(QString version READ version CONSTANT)

public:
    static Platform *instance();
    static void close();

    int brightness() const;
    void setBrightness(int value);
    bool blanking() const;
    void setBlanking(bool value);
    QString dbPath();
    void restart(char *argv[]);
    QString version();
    QString downloadDir();
    void information(const QString &label, QWidget *parent = 0);
    QString traceFileName();

signals:
    void brightnessChanged();
    void blankingChanged();
};

#endif // PLATFORM_H
