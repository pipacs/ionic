#ifndef PLATFORM_H
#define PLATFORM_H

#include <QString>
#include <QSize>

class QWidget;

/** Platform abstractions. */
class Platform
{
public:
    static Platform *instance();
    static void close();

    QString dbPath();
    QString icon(const QString &name, const QString &externsion = ".png");
    void restart(char *argv[]);
    QString version();
    QString downloadDir();
    QString defaultFont();
    static int defaultZoom();
    QString defaultOrientation();
    void setOrientation(QWidget *widget, const QString &orientation);
    void information(const QString &label, QWidget *parent = 0);
    void showBusy(QWidget *w, bool isBusy);
    QString traceFileName();
    static int softKeyHeight();
    static int toolBarIconHeight();
    static QSize size();
    static QSize availableSize();
};

#endif // PLATFORM_H
