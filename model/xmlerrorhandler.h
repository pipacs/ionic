#ifndef XMLERRORHANDLER_H
#define XMLERRORHANDLER_H

#include <QXmlErrorHandler>
#include <QDebug>

/** Generic XML error handler. */
class XmlErrorHandler: public QXmlErrorHandler {
    bool error(const QXmlParseException &e) {
        qCritical() << "XmlErrorHandler::error" << e.message() << "at line" << e.lineNumber();
        return true;
    }
    QString errorString() const {return QString();}
    bool fatalError(const QXmlParseException &e) {
        qCritical() << "XmlErrorHandler::fatalError" << e.message()
                << "at line" <<  e.lineNumber();
        return true;
    }
    bool warning(const QXmlParseException &e) {
        qWarning() << "XmlErrorHandler::warning" << e.message() << "at line" << e.lineNumber();
        return true;
    }
};

#endif // XMLERRORHANDLER_H
