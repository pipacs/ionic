#ifndef BOOKFINDER_H
#define BOOKFINDER_H

#include <QObject>
#include <QThread>

class QStringList;
class QString;

/** Find new books in a folder. */
class BookFinder: public QObject
{
    Q_OBJECT

public:
    explicit BookFinder(QObject *parent = 0);

public slots:
    void find(const QString &directory, const QStringList &books);

signals:
    void begin(int count);
    void add(const QString &bookPath);
    void done(int count);
};

#endif // BOOKFINDER_H
