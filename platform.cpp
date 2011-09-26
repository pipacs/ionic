#include <QtGlobal>
#include <QDir>
#include <QtGui>

#if defined(Q_OS_UNIX) && !defined(Q_OS_SYMBIAN)
#   include <unistd.h>
#endif

#ifdef Q_OS_SYMBIAN
#   include <eikenv.h>
#   include <eikappui.h>
#   include <aknenv.h>
#   include <aknappui.h>
#endif

#include "platform.h"
#include "trace.h"

#if defined(Q_OS_WIN32) || defined(Q_OS_SYMBIAN)
#   define DORIAN_BASE "dorian"
#else
#   define DORIAN_BASE ".dorian"
#endif

#if defined(Q_WS_MAC)
#   define DORIAN_ICON_PREFIX ":/icons/mac/"
#elif defined(Q_OS_SYMBIAN)
#   define DORIAN_ICON_PREFIX ":/icons/symbian/"
#elif defined(Q_WS_MAEMO_5)
#   define DORIAN_ICON_PREFIX ":/icons/maemo/"
#else
#   define DORIAN_ICON_PREFIX ":/icons/"
#endif

static const char *DORIAN_VERSION =
#include "pkg/version.txt"
;

#define DORIAN_LOG "dorian.txt"

#ifdef Q_WS_MAEMO_5
#   include <QtMaemo5/QMaemo5InformationBox>
#else
#   include <QMessageBox>
#endif

static Platform *theInstance;

Platform *Platform::instance()
{
    if (!theInstance) {
        theInstance = new Platform();
    }
    return theInstance;
}

void Platform::close()
{
    delete theInstance;
    theInstance = 0;
}

QString Platform::dbPath()
{
    QString base(QDir::home().absoluteFilePath(DORIAN_BASE));
    return QDir(base).absoluteFilePath("books.db");
}

QString Platform::icon(const QString &name, const QString &extension)
{
    QString iconName = QString(DORIAN_ICON_PREFIX) + name + extension;
    if (QFile(iconName).exists()) {
        return iconName;
    } else {
        return QString(":/icons/") + name + extension;
    }
}

void Platform::restart(char *argv[])
{
#if defined(Q_OS_UNIX) && !defined(Q_OS_SYMBIAN)
    extern char **environ;
    execve(argv[0], argv, environ);
#else
    Q_UNUSED(argv);
#endif
}

QString Platform::version()
{
    return QString(DORIAN_VERSION);
}

QString Platform::downloadDir()
{
#ifdef Q_OS_SYMBIAN
    if (QDir("E:/").exists()) {
        return "E:/Books";
    }
    return "C:/Books";
#else
    return QDir::home().absoluteFilePath("Books");
#endif
}

QString Platform::defaultFont()
{
#if defined(Q_WS_MAEMO_5) || defined(Q_WS_X11)
    return QString("Serif");
#elif defined(Q_WS_MAC)
    return QString("Hoefler Text");
#elif defined Q_WS_S60
    return QString("Nokia Sans S60");
#else
    return QString("Times New Roman");
#endif
}

void Platform::information(const QString &label, QWidget *parent)
{
#ifdef Q_WS_MAEMO_5
    QMaemo5InformationBox::information(parent, label,
                                       QMaemo5InformationBox::DefaultTimeout);
#else
    (void)QMessageBox::information(parent, QObject::tr("Dorian"), label,
                                   QMessageBox::Ok);
#endif
}

void Platform::showBusy(QWidget *w, bool isBusy)
{
#ifdef Q_WS_MAEMO_5
    w->setAttribute(Qt::WA_Maemo5ShowProgressIndicator, isBusy);
#else
    Q_UNUSED(w);
    Q_UNUSED(isBusy);
#endif
}

QString Platform::traceFileName()
{
    return QDir::home().absoluteFilePath(DORIAN_LOG);
}

int Platform::defaultZoom()
{
    return 150;
}

QString Platform::defaultOrientation()
{
#ifdef Q_OS_SYMBIAN
    return QString("portrait");
#else
    return QString("landscape");
#endif
}

void Platform::setOrientation(QWidget *widget, const QString &orientation)
{
    TRACE;
    qDebug() << "To" << orientation;

    Q_UNUSED(widget);

#if defined(Q_OS_SYMBIAN)
    CAknAppUi *appUi = dynamic_cast<CAknAppUi *>(CEikonEnv::Static()->AppUi());
    if (!appUi) {
        qCritical() << "Platform::setOrientation: Couldn't get AppUi pointer";
        return;
    }
#endif

    if (orientation == "portrait") {
#if defined(Q_WS_MAEMO_5)
        widget->setAttribute(Qt::WA_Maemo5LandscapeOrientation, false);
        widget->setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
#elif defined(Q_OS_SYMBIAN)
        TRAPD(error,
              appUi->SetOrientationL(CAknAppUi::EAppUiOrientationPortrait););
#endif
    } else {
#if defined(Q_WS_MAEMO_5)
        widget->setAttribute(Qt::WA_Maemo5PortraitOrientation, false);
        widget->setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
#elif defined(Q_OS_SYMBIAN)
        TRAPD(error,
              appUi->SetOrientationL(CAknAppUi::EAppUiOrientationLandscape););
#endif
    }
}

int Platform::softKeyHeight()
{
#if defined(Q_OS_SYMBIAN)
    return 62;
#else
    return 0;
#endif
}

int Platform::toolBarIconHeight()
{
#if defined(Q_OS_SYMBIAN)
    return 60;
#elif defined(Q_WS_X11)  && !defined(Q_WS_MAEMO_5)
    return 40;
#else
    return 0;
#endif
}

QSize Platform::size()
{
    return QApplication::desktop()->geometry().size();
}

QSize Platform::availableSize()
{
    QSize s = QApplication::desktop()->availableGeometry().size();
#if defined(Q_OS_SYMBIAN)
    // Work around a Qt bug on Symbian which sometimes forgets to reduce the
    // available height by the soft key area height
    if (s.height() == 548) {
        s.setHeight(s.height() - softKeyHeight());
    }
#endif
    return s;
}
