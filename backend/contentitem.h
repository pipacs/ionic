#ifndef CONTENTITEM_H
#define CONTENTITEM_H

/** An EPUB content item. */
struct ContentItem {
    ContentItem(): size(0) {}
    ContentItem(const QString &aName, const QString &aHref, const QString &aType = QString()) {
        size = 0; name = aName; href = aHref; mediaType = aType;
    }
    QString name;
    QString href;
    qint64 size;
    QString mediaType;
};

#endif // CONTENTITEM_H
