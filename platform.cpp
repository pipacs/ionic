#include <unistd.h>

#include <QProcess>
#include <QtGlobal>
#include <QDir>

#include "trace.h"
#include "platform.h"

#define IONIC_BASE ".dorian"
#define IONIC_LOG "dorian.txt"

static const char *IONIC_VERSION =
#include "pkg/version.txt"
;

static Platform *theInstance;

Platform::Platform(): QObject(), displayState(0) {
}

Platform::~Platform() {
    delete displayState;
}

int Platform::brightness() {
    int result = 3;
    QProcess gconftool;
    gconftool.start("gconftool-2", QStringList() << "-g" << "/system/osso/dsm/display/display_brightness");
    if (gconftool.waitForFinished()) {
        QByteArray output = gconftool.readAll();
        result = output[0] - '0';
        qDebug() << "Current brightness [" << result << "]";
    }
    return result;
}

void Platform::setBrightness(int value) {
    TRACE;
    QProcess gconftool;
    QString num;
    num.setNum(value);
    QStringList args = QStringList() << "-s" << "/system/osso/dsm/display/display_brightness" << "-t" << "int" << num;
    qDebug() << "Setting brightness: " << args;
    gconftool.start("gconftool-2", args);
    gconftool.waitForFinished();
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

void Platform::restart(char *argv[]) {
    extern char **environ;
    execve(argv[0], argv, environ);
}

QString Platform::version() {
    return QString(IONIC_VERSION);
}

QString Platform::downloadDir() {
    return QDir::home().absoluteFilePath("Books");
}

QString Platform::traceFileName() {
    return QDir::home().absoluteFilePath(IONIC_LOG);
}

void Platform::pauseBlanking() {
    if (!displayState) {
        displayState = new MeeGo::QmDisplayState;
    }
    (void)displayState->setBlankingPause();
}
