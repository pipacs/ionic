#ifndef TRACE_H
#define TRACE_H

#include <QtGlobal>
#include <QtDebug>
#include <QString>
#include <QTime>
#include <QEvent>
#include <QFile>

#ifdef Q_OS_SYMBIAN
#   ifdef __PRETTY_FUNCTION__
#       define TRACE Trace _(__PRETTY_FUNCTION__)
#   else
#       define TRACE Trace _(__FUNCTION__)
#   endif
#else
#   define TRACE Trace _(Q_FUNC_INFO)
#endif

/** Trace helper. */
class Trace {
public:
    Trace(const QString &s);
    ~Trace();
    static QString event(QEvent::Type t);
    static void messageHandler(QtMsgType type, const char *msg);
    static QtMsgType level;
    static void setFileName(const QString &fileName);
    static QString fileName();

protected:
    static QString prefix();
    QString name;
    static int indent;
    typedef struct {int type; const char *name;} EventName;
    static EventName eventTab[];
    static QFile file;
};

#endif // TRACE_H
