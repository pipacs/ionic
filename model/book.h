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

#include "bookmark.h"

class QPixmap;

/** A book. */
class Book: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_PROPERTY(QString rootPath READ rootPath NOTIFY rootPathChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QStringList parts READ parts NOTIFY partsChanged)
    Q_PROPERTY(QImage cover READ cover NOTIFY coverChanged)
    Q_PROPERTY(QStringList creators READ creators NOTIFY creatorsChanged)
    Q_PROPERTY(QString date READ date NOTIFY dateChanged)
    Q_PROPERTY(QString publisher READ publisher NOTIFY publisherChanged)
    Q_PROPERTY(QString datePublished READ datePublished NOTIFY datePublishedChanged)
    Q_PROPERTY(QString subject READ subject NOTIFY subjectChanged)
    Q_PROPERTY(QString source READ source NOTIFY sourceChanged)
    Q_PROPERTY(QString rights READ rights NOTIFY rightsChanged)
    Q_PROPERTY(QStringList chapters READ chapters NOTIFY chaptersChanged)
    Q_PROPERTY(qint64 size READ size NOTIFY sizeChanged)
    Q_PROPERTY(QDateTime dateAdded READ dateAdded NOTIFY dateAddedChanged)
    Q_PROPERTY(QDateTime dateOpened READ dateOpened NOTIFY dateOpenedChanged)
    Q_PROPERTY(Bookmark *lastBookmark READ lastBookmark NOTIFY lastBookmarkChanged)
    Q_PROPERTY(int partCount READ partCount)

public:

    /** Content item: An individual, named part of the book. */
    struct ContentItem {
        QString href;
        QString name;
        qint64 size;
    };

    /** Default constructor. */
    Book();

    /** Construct a book from an EPUB file. */
    Book(const QString &fileName, QObject *parent = 0);

    /** Destructor. */
    ~Book();

    /** Load book meta-data from persistent storage. */
    void load();

    /** Save book meta-data to persistent storage. */
    Q_INVOKABLE void save();

    /** Delete book meta-data from persistent storage. */
    void remove();

    /** Extract and parse EPUB contents, fill in all members except mPath. */
    bool open();

    /** Extract and parse metadata only, fill in all members except mPath. */
    void peek();

    /** Clear toc and content members, remove extracted content files. */
    void close();

    /** Set path to EPUB. */
    void setPath(const QString &path);

    /** Return path to EPUB. */
    QString path() const;

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
    void setLastBookmark(int part, qreal position);

    /** Set last bookmark. */
    void setLastBookmark(Bookmark *bookmark);

    /** Get last bookmark. */
    Bookmark *lastBookmark();

    /** Add bookmark. */
    void addBookmark(int part, qreal position, const QString &note);

    /** Change a given bookmark's note text */
    void setBookmarkNote(int index, const QString &note);

    /** Delete bookmark. */
    void deleteBookmark(int index);

    /** List bookmarks. */
    QList<Bookmark *> bookmarks();

    /**
     * Get friendly name.
     * @return @see title or path name combined with author(s) name.
     */
    QString name();

    /** Get short friendly name: title or file name. */
    QString shortName();

    /** Get chapter index from part index. */
    int chapterFromPart(int index);

    /** Get part index and URL fragment from chapter index. */
    int partFromChapter(int index, QString &fragment);

    /** Get progress (0..1) corresponding to part index and part position. */
    qreal getProgress(int part, qreal position);

    QString title() {load(); return title_;}
    QStringList parts() {load(); return parts_;}
    QHash<QString, ContentItem> content() {load(); return content_;}
    QImage cover() {load(); return cover_;}
    QStringList creators() {load(); return creators_;}
    QString date() {load(); return date_;}
    QString publisher() {load(); return publisher_;}
    QString datePublished() {load(); return datePublished_;}
    QString subject() {load(); return subject_;}
    QString source() {load(); return source_;}
    QString rights() {load(); return rights_;}
    QString tocPath() {load(); return tocPath_;}
    QString coverPath() {load(); return coverPath_;}
    QStringList chapters() {load(); return chapters_;}
    qint64 size() {load(); return size_;}
    QDateTime dateAdded() {load(); return dateAdded_;}
    QDateTime dateOpened() {load(); return dateOpened_;}

    void setTitle(const QString &title) {title_ = title; emit titleChanged();}
    void addPart(const QString &part) {
        parts_.append(part);
        emit partsChanged();
    }
    void clearParts() {parts_.clear(); emit partsChanged();}
    void addContent(const QString &key, ContentItem &item) {
        content_[key] = item;
        emit contentChanged();
    }
    void clearContent() {content_.clear(); emit contentChanged();}
    void setCover(const QImage &cover) {cover_ = cover; emit coverChanged();}
    void addCreator(const QString &creator) {
        creators_.append(creator);
        emit creatorsChanged();
    }
    void clearCreators() {creators_.clear(); emit creatorsChanged();}
    void setCreators(const QStringList &creators) {
        creators_ = creators;
        emit creatorsChanged();
    }
    void setDate(const QString &date) {date_ = date; emit dateChanged();}
    void setPublisher(const QString &publisher) {
        publisher_ = publisher;
        emit publisherChanged();
    }
    void setDatePublished(const QString date) {
        datePublished_ = date;
        emit datePublishedChanged();
    }
    void setSubject(const QString &subject) {
        subject_ = subject;
        emit subjectChanged();
    }
    void setSource(const QString &source) {
        source_ = source;
        emit sourceChanged();
    }
    void setRights(const QString &rights) {
        rights_ = rights;
        emit rightsChanged();
    }
    void setTocPath(const QString &tocPath) {tocPath_ = tocPath;}
    void setCoverPath(const QString &path) {coverPath_ = path;}
    void addChapter(const QString &chapter) {
        chapters_.append(chapter);
        emit chaptersChanged();
    }
    void clearChapters() {chapters_.clear(); emit chaptersChanged();}
    void setSize(const quint64 size) {size_ = size; emit sizeChanged();}
    void setDateAdded(const QDateTime &date) {
        dateAdded_ = date;
        emit dateAddedChanged();
    }
    void setDateOpened(const QDateTime &date) {
        dateOpened_ = date;
        emit dateOpenedChanged();
    }
    void setRootPath(const QString &path) {
        rootPath_ = path;
        emit rootPathChanged();
    }

    /**
     * Is this a valid book?
     * @return True if the book represents a valid EPUB file, false otherwise.
     */
    bool isValid() const;

    /** Get the URL corresponding to a given part. */
    Q_INVOKABLE QString url(int part);

    /** Get the number of parts. */
    int partCount();

signals:
    /** Emitted if @see open() succeeds. */
    void opened(const QString &bookPath);

    /* Property change notifications */
    void pathChanged();
    void rootPathChanged();
    void titleChanged();
    void partsChanged();
    void contentChanged();
    void coverChanged();
    void creatorsChanged();
    void dateChanged();
    void publisherChanged();
    void datePublishedChanged();
    void subjectChanged();
    void sourceChanged();
    void rightsChanged();
    void chaptersChanged();
    void sizeChanged();
    void dateAddedChanged();
    void dateOpenedChanged();
    void nameChanged();
    void shortNameChanged();
    void lastBookmarkChanged();

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

    QString title_;                         //< Book title from EPUB.
    QStringList parts_;                     //< EPUB part list.
    QHash<QString, ContentItem> content_;   //< Content items from EPUB.
    QImage cover_;                          //< Cover image.
    QStringList creators_;                  //< Creators.
    QString date_;                          //< Date of creation.
    QString publisher_;                     //< Publisher.
    QString datePublished_;                 //< Date of publishing.
    QString subject_;                       //< Subject.
    QString source_;                        //< Source.
    QString rights_;                        //< Rights.
    QString tocPath_;                       //< Path to toc NCX file.
    QString coverPath_;                     //< Path to cover HTML file.
    QStringList chapters_;                  //< Main navigation items.
    qint64 size_;                           //< Size of all parts.
    QDateTime dateAdded_;                   //< Date book added to library.
    QDateTime dateOpened_;                  //< Date book was last read.
    QString path_;                          //< Path to EPUB file.
    Bookmark *lastBookmark_;                //< Last position read.
    QList<Bookmark *> bookmarks_;           //< List of bookmarks.
    QString rootPath_;                      //< Path to root item in EPUB dir.
    QTemporaryFile tempFile_;               //< Guards extracting books.
    bool loaded_;                           //< True, if loaded from database.
    bool valid_;                            //< True, if the book represents a valid EPUB file.
    bool isOpen_;                           //< True, if the book is open.
};

#endif // BOOK_H
