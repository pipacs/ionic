#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QImage>
#include <QMetaType>
#include <QObject>
#include <QTemporaryFile>
#include <QDateTime>

class QPixmap;

/** A book. */
class Book: public QObject
{
    Q_OBJECT

public:

    /** Content item: An individual, named part of the book. */
    struct ContentItem
    {
        QString href;
        QString name;
        qint64 size;
    };

    /** Bookmark: a volume index and a relative position in volume. */
    struct Bookmark
    {
        Bookmark(int part_, qreal pos_, const QString &note_ = QString()):
                part(part_), pos(pos_), note(note_) {}
        Bookmark(): part(0), pos(0.0) {}
        int part;
        qreal pos;
        QString note;
        bool operator<(const Bookmark &other) const {
            return (part == other.part)? (pos<other.pos): (part<other.part);
        }
    };

    /** Construct a book from an EPUB file. */
    Book(const QString &fileName, QObject *parent = 0);

    /** Destructor. */
    ~Book();

    /** Load book meta-data from persistent storage. */
    void load();

    /** Save book meta-data to persistent storage. */
    void save();

    /** Upgrade persistent storage of book meta-data. */
    void upgrade();

    /** Delete book meta-data from persistent storage. */
    void remove();

    /** Extract and parse EPUB contents, fill in all members except mPath. */
    bool open();

    /** Extract and parse metadata only, fill in all members except mPath. */
    void peek();

    /** Clear toc and content members, remove extracted content files. */
    void close();

    /** Return path to EPUB. */
    QString path();

    /**
     * Return path to root directory of extracted EPUB.
     * Only valid after parsing Book::opsPath().
     */
    QString rootPath();

    /** Return temporary directory path for extracting EPUB file. */
    QString tmpDir() const;

    /** Clear directory. */
    bool clearDir(const QString &directory);

    /** Set last bookmark. */
    void setLastBookmark(int part, qreal position, bool fast = false);

    /** Get last bookmark. */
    Bookmark lastBookmark();

    /** Add bookmark. */
    void addBookmark(int part, qreal position, const QString &note);

    /** Change a given bookmark's note text */
    void setBookmarkNote(int index, const QString &note);

    /** Delete bookmark. */
    void deleteBookmark(int index);

    /** List bookmarks. */
    QList<Bookmark> bookmarks();

    /**
     * Get friendly name.
     * @return @see title or path name combined with author(s) name.
     */
    QString name();

    /** Get cover image. */
    QImage coverImage();

    /** Get short friendly name: title or file name. */
    QString shortName();

    /** Get chapter index from part index. */
    int chapterFromPart(int index);

    /** Get part index and URL fragment from chapter index. */
    int partFromChapter(int index, QString &fragment);

    /** Get progress (0..1) corresponding to part index and part position. */
    qreal getProgress(int part, qreal position);

    QString title;                          //< Book title from EPUB.
    QStringList parts;                      //< EPUB part list.
    QHash<QString, ContentItem> content;    //< Content items from EPUB.
    QImage cover;                           //< Cover image.
    QStringList creators;                   //< Creators.
    QString date;                           //< Date of creation.
    QString publisher;                      //< Publisher.
    QString datePublished;                  //< Date of publishing.
    QString subject;                        //< Subject.
    QString source;                         //< Source.
    QString rights;                         //< Rights.
    QString tocPath;                        //< Path to toc NCX file.
    QString coverPath;                      //< Path to cover HTML file.
    QStringList chapters;                   //< Main navigation items.
    qint64 size;                            //< Size of all parts.
    QDateTime dateAdded;                    //< Date book added to library.
    QDateTime dateOpened;                   //< Date book was last read.

signals:
    /** Emitted if @see open() succeeds. */
    void opened(const QString &bookPath);

protected:
    /** Extract EPUB as ZIP. */
    bool extract(const QStringList &excludedExtensions);

    /** Extract metadata from EPUB. */
    bool extractMetaData();

    /** Parse extracted EPUB. */
    bool parse();

    /** Clear all book fields except path. */
    void clear();

    /** Get location of OPS file in EPUB archive. */
    QString opsPath();

    /** Make a cover image from a file. */
    QImage makeCover(const QString &fileName);

    /** Make a cover image from an pixmap. */
    QImage makeCover(const QPixmap &pixmap);

    QString mPath;                          //< Path to EPUB file.
    Bookmark mLastBookmark;                 //< Last position read.
    QList<Bookmark> mBookmarks;             //< List of bookmarks.
    QString mRootPath;                      //< Path to root item in EPUB dir.
    QTemporaryFile mTempFile;               //< Guards extracting books.
    bool loaded;                            //< True, if loaded from database.
};

#endif // BOOK_H
