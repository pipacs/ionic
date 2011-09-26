#ifndef CONTAINERHANDLER_H
#define CONTAINERHANDLER_H

#include "xmlhandler.h"

/** XML content handler for EPUB container format. */
class ContainerHandler: public XmlHandler {
public:
    bool endElement(const QString &, const QString &, const QString &) {
        return true;
    }

    bool startElement(const QString &namespaceUri, const QString &name,
                      const QString &qName, const QXmlAttributes &attrs)
    {
        (void)namespaceUri;
        (void)qName;
        if (name == "rootfile") {
            rootFile = attrs.value("full-path");
        }
        return true;
    }

    QString rootFile;
};

#endif // CONTAINERHANDLER_H
