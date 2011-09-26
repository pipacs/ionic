#ifndef NCXHANDLER_H
#define NCXHANDLER_H

#include "xmlhandler.h"
#include "book.h"
#include "trace.h"

/** XML content handler for NCX format. */
class NcxHandler: public XmlHandler
{
public:
    struct TreeItem
    {
        TreeItem(const QString &i, TreeItem *p = 0): id(i), parent(p) {
            if (parent) {
                parent->children.append(this);
                depth = parent->depth + 1;
            } else {
                depth = 0;
            }
        }
        ~TreeItem() {
            qDeleteAll(children);
        }
        void addToBook(Book &book) {
            Book::ContentItem contentItem;
            contentItem.href = href;
            contentItem.name = QString(" ").repeated(depth) + name;
            contentItem.size = 0;
            book.content[id] = contentItem;
            book.chapters.append(id);
            qDebug() << "TreeItem::addToBook" << id << contentItem.href
                    << contentItem.name;
            foreach (TreeItem *child, children) {
                child->addToBook(book);
            }
        }
        QList<TreeItem *> children;
        QString id;
        QString href;
        QString name;
        TreeItem *parent;
        int depth;
    };

    NcxHandler(Book &b): book(b), rootItem(0), currentItem(0) {
        book.chapters.clear();
    }

    ~NcxHandler() {
        delete rootItem;
    }

    bool endElement(const QString &namespaceUri, const QString &name,
                    const QString &qName) {
        (void)namespaceUri;
        (void)qName;
        if (name == "text") {
            if (currentItem) {
                if (currentItem) {
                    currentItem->name = currentText;
                }
            }
        } else if (name == "navPoint") {
            if (currentItem) {
                currentItem = currentItem->parent;
                if (currentItem == 0) {
                    // Root navigation point reached, dump TOC tree
                    rootItem->addToBook(book);
                    delete rootItem;
                    rootItem = 0;
                }
            }
        }
        return true;
    }

    bool startElement(const QString &namespaceUri, const QString &name,
                      const QString &qName, const QXmlAttributes &attrs) {
        (void)namespaceUri;
        (void)qName;
        currentText = "";
        if (name == "navPoint") {
            TreeItem *item = new TreeItem(attrs.value("id"), currentItem);
            if (!rootItem) {
                rootItem = item;
            }
            currentItem = item;
        } else if (name == "content") {
            if (currentItem) {
                currentItem->href = attrs.value("src");
            }
        }
        return true;
    }

private:
    Book &book;
    QString contentId;
    QString contentUrl;
    QString contentTitle;
    TreeItem *rootItem;
    TreeItem *currentItem;
};

#endif // NCXHANDLER_H
