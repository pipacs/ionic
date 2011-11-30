#include <QEvent>
#include <QFile>

#include "trace.h"

int Trace::indent;
QtMsgType Trace::level = QtDebugMsg;
QFile Trace::file;

Trace::Trace(const QString &s): name(s) {
    messageHandler(QtDebugMsg, QString(">%1").arg(name).toAscii().constData());
    indent++;
}

Trace::~Trace() {
    if (--indent < 0) {
        indent = 0;
    }
    messageHandler(QtDebugMsg, QString("<%1").arg(name).toAscii().constData());
}

QString Trace::prefix() {
    return QString(" ").repeated(indent);
}

void Trace::messageHandler(QtMsgType type, const char *msg) {
    if (type >= Trace::level) {
        QtMsgHandler oldHandler = qInstallMsgHandler(0);
        switch (type) {
        case QtDebugMsg:
            qt_message_output(QtDebugMsg, (prefix()+msg).toUtf8().constData());
            break;
        default:
            qt_message_output(type, msg);
        }
        qInstallMsgHandler(oldHandler);
        if (Trace::file.isOpen()) {
            Trace::file.write((prefix() + msg + "\n").toUtf8());
            Trace::file.flush();
        }
    }
}

void Trace::setFileName(const QString &fileName) {
    Trace::file.close();
    Trace::file.setFileName(fileName);
    if (!fileName.isEmpty()) {
        (void)Trace::file.open(QIODevice::WriteOnly);
    }
}

QString Trace::fileName() {
    return Trace::file.fileName();
}
