#ifndef CONTENTITEM_H
#define CONTENTITEM_H

#include <QObject>
#include <QMetaType>

/** An EPUB content item. */
class ContentItem: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString href READ getHref)
    Q_PROPERTY(QString name READ getName)

public:
    ContentItem(): QObject(0), size(0) {}
    ContentItem(QObject *parent): QObject(parent), size(0) {}
    ContentItem(const QString &aName, const QString &aHref): QObject(0), size(0) {name = aName; href = aHref;}
    QString getName() {return name;}
    QString getHref() {return href;}
    QString name;
    QString href;
    qint64 size;

signals:
public slots:
};

Q_DECLARE_METATYPE(ContentItem *)

#endif // CONTENTITEM_H
