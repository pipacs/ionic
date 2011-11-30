#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QXmlContentHandler>

/** Generic XML handler. */
class XmlHandler: public QXmlContentHandler {
public:
    bool endDocument() {return true;}
    bool endPrefixMapping(const QString &) {return true;}
    QString errorString() const {return QString();}
    bool ignorableWhitespace(const QString &) {return true;}
    bool processingInstruction(const QString &, const QString &) {
        return true;
    }
    void setDocumentLocator(QXmlLocator *) {}
    bool skippedEntity(const QString &) {return true;}
    bool startDocument() {return true;}
    bool startPrefixMapping(const QString &, const QString &) {return true;}

    bool characters(const QString &ch) {
        currentText += ch;
        return true;
    }

protected:
    QString currentText;
};

#endif // XMLHANDLER_H
