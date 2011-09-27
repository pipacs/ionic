#include <qtextdocument.h>  // Qt::escape is currently defined here...
#include <QtGui>

#include "book.h"
#include "opshandler.h"
#include "xmlerrorhandler.h"
#include "extractzip.h"
#include "library.h"
#include "containerhandler.h"
#include "ncxhandler.h"
#include "trace.h"
#include "bookdb.h"

const int COVER_WIDTH = 53;
const int COVER_HEIGHT = 59;
const int COVER_MAX = 512 * 1024;

Book::Book(): QObject(0), path_(""), loaded_(false) {
}

Book::Book(const QString &p, QObject *parent): QObject(parent), loaded_(false) {
    setPath(p);
}

Book::~Book() {
    close();
}

void Book::setPath(const QString &p) {
    path_ = "";
    if (p.size()) {
        QFileInfo info(p);
        path_ = info.absoluteFilePath();
        title_ = info.baseName();
        tempFile_.open();
    }
    emit pathChanged();
}

QString Book::path() {
    return path_;
}

bool Book::open() {
    TRACE;
    qDebug() << path();
    close();
    clear();
    load();
    if (path().isEmpty()) {
        setTitle("No book");
        return false;
    }
    if (!extract(QStringList())) {
        return false;
    }
    if (!parse()) {
        return false;
    }
    setDateOpened(QDateTime::currentDateTime().toUTC());
    save();
    emit opened(path());
    return true;
}

void Book::peek() {
    TRACE;
    qDebug() << path();
    close();
    clear();
    load();
    if (path().isEmpty()) {
        setTitle("No book");
        return;
    }
    if (!extractMetaData()) {
        return;
    }
    if (!parse()) {
        return;
    }
    save();
    close();
}

void Book::close() {
    Trace t("Book::close");
    clearContent();
    clearParts();
    QDir::setCurrent(QDir::rootPath());
    clearDir(tmpDir());
}

QString Book::tmpDir() const {
    QString tmpName = QFileInfo(tempFile_.fileName()).fileName();
    return QDir(QDir::temp().absoluteFilePath("dorian")).
            absoluteFilePath(tmpName);
}

bool Book::extract(const QStringList &excludedExtensions) {
    TRACE;
    bool ret = false;
    QString tmp = tmpDir();
    qDebug() << "Extracting" << path_ << "to" << tmp;

    load();
    QDir::setCurrent(QDir::rootPath());
    if (!clearDir(tmp)) {
        qCritical() << "Book::extract: Failed to remove" << tmp;
        return false;
    }
    QDir d(tmp);
    if (!d.exists()) {
        if (!d.mkpath(tmp)) {
            qCritical() << "Book::extract: Could not create" << tmp;
            return false;
        }
    }

    // If book comes from resource, copy it to the temporary directory first
    QString bookPath = path();
    if (bookPath.startsWith(":/books/")) {
        QFile src(bookPath);
        QString dst(QDir(tmp).absoluteFilePath("book.epub"));
        if (!src.copy(dst)) {
            qCritical() << "Book::extract: Failed to copy built-in book"
                    << bookPath << "to" << dst;
            return false;
        }
        bookPath = dst;
    }

    QString oldDir = QDir::currentPath();
    if (!QDir::setCurrent(tmp)) {
        qCritical() << "Book::extract: Could not change to" << tmp;
        return false;
    }
    ret = extractZip(bookPath, excludedExtensions);
    if (!ret) {
        qCritical() << "Book::extract: Extracting ZIP failed";
    }
    QDir::setCurrent(oldDir);
    return ret;
}

bool Book::parse() {
    TRACE;

    load();

    // Parse OPS file
    bool ret = false;
    QString opsFileName = opsPath();
    qDebug() << "Parsing OPS file" << opsFileName;
    QFile opsFile(opsFileName);
    QXmlSimpleReader reader;
    QXmlInputSource *source = new QXmlInputSource(&opsFile);
    OpsHandler *opsHandler = new OpsHandler(*this);
    XmlErrorHandler *errorHandler = new XmlErrorHandler();
    reader.setContentHandler(opsHandler);
    reader.setErrorHandler(errorHandler);
    ret = reader.parse(source);
    delete errorHandler;
    delete opsHandler;
    delete source;

    // Initially, put all content items in the chapter list.
    // This will be refined by parsing the NCX file later
    chapters_ = parts_;
    emit chaptersChanged();

    // Load cover image
    QString coverPath;
    QStringList coverKeys;
    coverKeys << "cover-image" << "img-cover-jpeg" << "cover";
    foreach (QString key, coverKeys) {
        if (content_.contains(key)) {
            coverPath = QDir(rootPath()).absoluteFilePath(content_[key].href);
            break;
        }
    }
    if (coverPath.isEmpty()) {
        // Last resort
        QString coverJpeg = QDir(rootPath()).absoluteFilePath("cover.jpg");
        if (QFileInfo(coverJpeg).exists()) {
            coverPath = coverJpeg;
        }
    }
    if (!coverPath.isEmpty()) {
        qDebug() << "Loading cover image from" << coverPath;
        setCover(makeCover(coverPath));
    }

    // If there is an "ncx" item in content, parse it: That's the real table
    // of contents
    QString ncxFileName;
    if (content_.contains("ncx")) {
        ncxFileName = content_["ncx"].href;
    } else if (content_.contains("ncxtoc")) {
        ncxFileName = content_["ncxtoc"].href;
    } else if (content_.contains("toc")) {
        ncxFileName = content_["toc"].href;
    } else {
        qDebug() << "No NCX table of contents";
    }
    if (!ncxFileName.isEmpty()) {
        qDebug() << "Parsing NCX file" << ncxFileName;
        QFile ncxFile(QDir(rootPath()).absoluteFilePath(ncxFileName));
        source = new QXmlInputSource(&ncxFile);
        NcxHandler *ncxHandler = new NcxHandler(*this);
        errorHandler = new XmlErrorHandler();
        reader.setContentHandler(ncxHandler);
        reader.setErrorHandler(errorHandler);
        ret = reader.parse(source);
        delete errorHandler;
        delete ncxHandler;
        delete source;
    }

    // Calculate book part sizes and total size
    size_ = 0;
    foreach (QString part, parts_) {
        QFileInfo info(QDir(rootPath()).absoluteFilePath(content_[part].href));
        content_[part].size = info.size();
        size_ += content_[part].size;
    }
    emit sizeChanged();

    return ret;
}

bool Book::clearDir(const QString &dir) {
    QDir d(dir);
    if (!d.exists()) {
        return true;
    }
    QDirIterator i(dir, QDirIterator::Subdirectories);
    while (i.hasNext()) {
        QString entry = i.next();
        if (entry.endsWith("/.") || entry.endsWith("/..")) {
            continue;
        }
        QFileInfo info(entry);
        if (info.isDir()) {
            if (!clearDir(entry)) {
                return false;
            }
        }
        else {
            if (!QFile::remove(entry)) {
                qCritical() << "Book::clearDir: Could not remove" << entry;
                // FIXME: To be investigated: This is happening too often
                // return false;
            }
        }
    }
    (void)d.rmpath(dir);
    return true;
}

void Book::clear() {
    close();
    setTitle("");
    clearCreators();
    setDate("");
    setPublisher("");
    setDatePublished("");
    setSubject("");
    setSource("");
    setRights("");
}

void Book::load() {
    if (loaded_) {
        return;
    }

    TRACE;
    loaded_ = true;
    qDebug() << "path" << path();

    QVariantHash data = BookDb::instance()->load(path());
    setTitle(data["title"].toString());
    qDebug() << title_;
    setCreators(data["creators"].toStringList());
    setDate(data["date"].toString());
    setPublisher(data["publisher"].toString());
    setDatePublished(data["datepublished"].toString());
    setSubject(data["subject"].toString());
    setSource(data["source"].toString());
    setRights(data["rights"].toString());
    Bookmark bookmark(data["lastpart"].toInt(), data["lastpos"].toReal(), "");
    setLastBookmark(bookmark);
    setCover(data["cover"].value<QImage>());
    if (cover().isNull()) {
        setCover(makeCover(":/icons/book.png"));
    }
    int size = data["bookmarks"].toInt();
    for (int i = 0; i < size; i++) {
        int part = data[QString("bookmark%1part").arg(i)].toInt();
        qreal pos = data[QString("bookmark%1pos").arg(i)].toReal();
        QString note = data[QString("bookmark%1note").arg(i)].toString();
        bookmarks_.append(Bookmark(part, pos, note));
    }
    setDateAdded(data["dateadded"].toDateTime());
    setDateOpened(data["dateopened"].toDateTime());
}

void Book::save() {
    TRACE;

    load();
    QVariantHash data;
    data["title"] = title_;
    data["creators"] = creators_;
    data["date"] = date_;
    data["publisher"] = publisher_;
    data["datepublished"] = datePublished_;
    data["subject"] = subject_;
    data["source"] = source_;
    data["rights"] = rights_;
    data["lastpart"] = lastBookmark_.part();
    data["lastpos"] = lastBookmark_.pos();
    data["cover"] = cover_;
    data["bookmarks"] = bookmarks_.size();
    for (int i = 0; i < bookmarks_.size(); i++) {
        data[QString("bookmark%1part").arg(i)] = bookmarks_[i].part();
        data[QString("bookmark%1pos").arg(i)] = bookmarks_[i].pos();
        data[QString("bookmark%1note").arg(i)] = bookmarks_[i].note();
    }
    data["dateadded"] = dateAdded_;
    data["dateopened"] = dateOpened_;
    BookDb::instance()->save(path(), data);
}

void Book::setLastBookmark(int part, qreal position, bool fast) {
    TRACE;
    qDebug() << "Part" << part << "position" << position << "fast?" << fast;
    if (!fast) {
        load();
    }
    lastBookmark_.setPart(part);
    lastBookmark_.setPos(position);
    if (!fast) {
        save();
    }
    emit lastBookmarkChanged();
}

Bookmark Book::lastBookmark() {
    load();
    return lastBookmark_;
}

void Book::addBookmark(int part, qreal position, const QString &note) {
    load();
    bookmarks_.append(Bookmark(part, position, note));
    qSort(bookmarks_.begin(), bookmarks_.end());
    save();
}

void Book::setBookmarkNote(int index, const QString &note) {
    load();
    if (index >= 0 && index < bookmarks_.length()) {
        bookmarks_[index].setNote(note);
    }
    save();

}

void Book::deleteBookmark(int index) {
    load();
    bookmarks_.removeAt(index);
    save();
}

QList<Bookmark> Book::bookmarks() {
    load();
    return bookmarks_;
}

QString Book::opsPath() {
    TRACE;
    load();
    QString ret;

    QFile container(tmpDir() + "/META-INF/container.xml");
    qDebug() << container.fileName();
    QXmlSimpleReader reader;
    QXmlInputSource *source = new QXmlInputSource(&container);
    ContainerHandler *containerHandler = new ContainerHandler();
    XmlErrorHandler *errorHandler = new XmlErrorHandler();
    reader.setContentHandler(containerHandler);
    reader.setErrorHandler(errorHandler);
    if (reader.parse(source)) {
        ret = tmpDir() + "/" + containerHandler->rootFile;
        rootPath_ = QFileInfo(ret).absoluteDir().absolutePath();
        emit rootPathChanged();
        qDebug() << "OSP path" << ret << "\nRoot dir" << rootPath_;
    }
    delete errorHandler;
    delete containerHandler;
    delete source;
    return ret;
}

QString Book::rootPath() {
    load();
    return rootPath_;
}

QString Book::name() {
    load();
    if (title_.size()) {
        QString ret = title_;
        if (creators_.length()) {
            ret += "\nBy " + creators_.join(", ");
        }
        return ret;
    }
    return path();
}

QString Book::shortName() {
    load();
    return (title_.isEmpty())? QFileInfo(path()).baseName(): title_;
}

QImage Book::coverImage() {
    load();
    return cover_;
}

int Book::chapterFromPart(int index) {
    TRACE;
    load();
    int ret = -1;

    QString partId = parts_[index];
    QString partHref = content_[partId].href;

    for (int i = 0; i < chapters_.size(); i++) {
        QString id = chapters_[i];
        QString href = content_[id].href;
        int hashPos = href.indexOf("#");
        if (hashPos != -1) {
            href = href.left(hashPos);
        }
        if (href == partHref) {
            ret = i;
            // Don't break, keep looking
        }
    }

    qDebug() << "Part" << index << partId << partHref << ":" << ret;
    return ret;
}

int Book::partFromChapter(int index, QString &fragment) {
    TRACE;
    load();
    fragment.clear();
    QString id = chapters_[index];
    QString href = content_[id].href;
    int hashPos = href.indexOf("#");
    if (hashPos != -1) {
        fragment = href.mid(hashPos);
        href = href.left(hashPos);
    }

    qDebug() << "Chapter" << index;
    qDebug() << " id" << id;
    qDebug() << " href" << href;
    qDebug() << " fragment" << fragment;

    for (int i = 0; i < parts_.size(); i++) {
        QString partId = parts_[i];
        if (content_[partId].href == href) {
            qDebug() << "Part index for" << href << "is" << i;
            return i;
        }
    }

    qWarning() << "Book::partFromChapter: Could not find part index for"
        << href;
    return -1;
}

qreal Book::getProgress(int part, qreal position) {
    load();
    Q_ASSERT(part < parts.size());
    QString key;
    qreal partSize = 0;
    for (int i = 0; i < part; i++) {
        key = parts_[i];
        partSize += content_[key].size;
    }
    key = parts_[part];
    partSize += content_[key].size * position;
    return partSize / (qreal)size_;
}

bool Book::extractMetaData() {
    QStringList excludedExtensions;
    excludedExtensions << ".html" << ".xhtml" << ".xht" << ".htm" << ".gif"
            << ".css" << "*.ttf" << "mimetype";
    return extract(excludedExtensions);
}

void Book::remove() {
    TRACE;
    close();
    BookDb::instance()->remove(path());
}

QImage Book::makeCover(const QString &fileName) {
    TRACE;
    qDebug() << fileName;
    QFileInfo info(fileName);
    if (info.isReadable() && (info.size() < COVER_MAX)) {
        return makeCover(QPixmap(fileName));
    }
    return makeCover(QPixmap(":/icons/book.png"));
}

QImage Book::makeCover(const QPixmap &pixmap) {
    TRACE;
    QPixmap src = pixmap.scaled(COVER_WIDTH, COVER_HEIGHT,
        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap transparent(COVER_WIDTH, COVER_HEIGHT);
    transparent.fill(Qt::transparent);

    QPainter p;
    p.begin(&transparent);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawPixmap((COVER_WIDTH - src.width()) / 2,
                 (COVER_HEIGHT - src.height()) / 2, src);
    p.end();

    return transparent.toImage();
}
