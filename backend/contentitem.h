#ifndef CONTENTITEM_H
#define CONTENTITEM_H

/** An EPUB content item. */
struct ContentItem {
    ContentItem() {size = 0;}
    ContentItem(const QString &aName, const QString &aHref) {size = 0; name = aName; href = aHref;}
    QString name;
    QString href;
    qint64 size;
};

#endif // CONTENTITEM_H
