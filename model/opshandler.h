#ifndef OPSHANDLER_H
#define OPSHANDLER_H

#include "xmlhandler.h"
#include "book.h"
#include "trace.h"

/** XML content handler for OPS format. */
class OpsHandler: public XmlHandler {
public:
    OpsHandler(Book &b): book(b), partCount(0) {
        book.clearCreators();
    }

    bool endElement(const QString &namespaceUri, const QString &name, const QString &qName) {
        (void)namespaceUri;
        (void)qName;
        if (currentText.size()) {
            if (name == "title") {
                book.setTitle(currentText);
            } else if (name == "creator") {
                book.addCreator(currentText);
            } else if (name == "publisher") {
                book.setPublisher(currentText);
            } else if (name == "subject") {
                book.setSubject(currentText);
            } else if (name == "source") {
                book.setSource(currentText);
            } else if (name == "rights") {
                book.setRights(currentText);
            }
        }
        return true;
    }

    bool startElement(const QString &namespaceUri, const QString &name, const QString &qName, const QXmlAttributes &attrs) {
        (void)namespaceUri;
        (void)qName;
        currentText = "";

        if (name == "item") {
            QString href = attrs.value("href");
            QString name = QString("Part %1").arg(partCount + 1);
            QString key = attrs.value("id");
            book.addContent(key, name, href);
            partCount++;
        } else if (name == "itemref") {
            book.addPart(attrs.value("idref"));
        }
        return true;
    }

private:
    Book &book;
    int partCount;
};

#endif // OPSHANDLER_H
