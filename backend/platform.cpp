#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <QProcess>
#include <QtGlobal>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <qplatformdefs.h>

#if defined(MEEGO_EDITION_HARMATTAN)
#   include <qmdevicemode.h>
#   include <qmsysteminformation.h>
#endif

#include "trace.h"
#include "platform.h"

#define IONIC_BASE ".dorian"
#define IONIC_LOG "dorian.txt"

#if defined(Q_OS_SYMBIAN)
#define IONIC_OS_NAME "symbian"
#elif defined(MEEGO_EDITION_HARMATTAN)
#define IONIC_OS_NAME "harmattan"
#else
#error "Unsupported OS"
#endif

static Platform *theInstance;

Platform::Platform(): QObject()
#if defined(MEEGO_EDITION_HARMATTAN)
  , displayState(0)
#endif
{
}

Platform::~Platform() {
#if defined(MEEGO_EDITION_HARMATTAN)
    delete displayState;
#endif
}

int Platform::brightness() {
    int result = 3;
#if defined(MEEGO_EDITION_HARMATTAN)
    QProcess gconftool;
    gconftool.start("gconftool-2", QStringList() << "-g" << "/system/osso/dsm/display/display_brightness");
    if (gconftool.waitForFinished()) {
        QByteArray output = gconftool.readAll();
        result = output[0] - '0';
        qDebug() << "Current brightness [" << result << "]";
    }
#endif
    return result;
}

void Platform::setBrightness(int value) {
#if defined(MEEGO_EDITION_HARMATTAN)
    QProcess gconftool;
    QString num;
    num.setNum(value);
    QStringList args = QStringList() << "-s" << "/system/osso/dsm/display/display_brightness" << "-t" << "int" << num;
    gconftool.start("gconftool-2", args);
    gconftool.waitForFinished();
#else
    (void)value;
#endif
}

Platform *Platform::instance() {
    if (!theInstance) {
        theInstance = new Platform();
    }
    return theInstance;
}

void Platform::close() {
    delete theInstance;
    theInstance = 0;
}

QString Platform::dbPath() {
    QString base(QDir::home().absoluteFilePath(IONIC_BASE));
    return QDir(base).absoluteFilePath("books.db");
}

QString Platform::downloadDir() {
    return QDir::home().absoluteFilePath("Books");
}

QString Platform::traceFileName() {
    return QDir::home().absoluteFilePath(IONIC_LOG);
}

void Platform::pauseBlanking() {
#if defined(MEEGO_EDITION_HARMATTAN)
    if (!displayState) {
        displayState = new MeeGo::QmDisplayState;
    }
    (void)displayState->setBlankingPause();
#endif
}

QString Platform::text(const QString &key) {
    QFile resource(":/texts/" + key);
    resource.open(QFile::ReadOnly);
    QString ret = resource.readAll();
    resource.close();
    return ret;
}

void Platform::browse(const QString &url) {
    QDesktopServices::openUrl(QUrl(url));
}

QString Platform::osName() {
    return IONIC_OS_NAME;
}

QString Platform::osVersion() {
#if defined(Q_OS_SYMBIAN)
    return "3";
#elif defined(MEEGO_EDITION_HARMATTAN)
    QString firmwareVersion = MeeGo::QmSystemInformation().valueForKey("/device/sw-release-ver");
    if (!firmwareVersion.startsWith("DFL61_HARMATTAN_") || firmwareVersion.length() < 20) {
        return "unknown";
    }
    int majorVersion = firmwareVersion.at(16).digitValue();
    int minorVersion = firmwareVersion.at(17).digitValue();
    if (majorVersion >= 3) {
        return "1.2";
    } else if (majorVersion == 2 && minorVersion >= 2) {
        return "1.1.1";
    } else if (majorVersion == 2) {
        return "1.1";
    } else if (majorVersion == 1) {
        return "1.0";
    } else {
        return "unknown";
    }
#else
    return QString();
#endif
}
