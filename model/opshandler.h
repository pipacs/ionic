#ifndef OPSHANDLER_H
#define OPSHANDLER_H

#include "xmlhandler.h"
#include "book.h"
#include "trace.h"

/** XML content handler for OPS format. */
class OpsHandler: public XmlHandler {
public:
    OpsHandler(Book &b): book(b), partCount(0) {
        book.creators.clear();
    }

    bool endElement(const QString &namespaceUri, const QString &name,
                    const QString &qName) {
        (void)namespaceUri;
        (void)qName;
        if (currentText.size()) {
            if (name == "title") {
                book.title = currentText;
            } else if (name == "creator") {
                book.creators.append(currentText);
            } else if (name == "publisher") {
                book.publisher = currentText;
            } else if (name == "subject") {
                book.subject = currentText;
            } else if (name == "source") {
                book.source = currentText;
            } else if (name == "rights") {
                book.rights = currentText;
            }
        }
        return true;
    }

    bool startElement(const QString &namespaceUri, const QString &name,
                      const QString &qName, const QXmlAttributes &attrs) {
        (void)namespaceUri;
        (void)qName;
        currentText = "";

        if (name == "item") {
            Book::ContentItem item;
            item.href = attrs.value("href");
            item.name = QString("Part %1").arg(partCount + 1);
            item.size = 0;
            QString key = attrs.value("id");
            book.content[key] = item;
            partCount++;
        } else if (name == "itemref") {
            book.parts.append(attrs.value("idref"));
        }
        return true;
    }

private:
    Book &book;
    int partCount;
};

#endif // OPSHANDLER_H
