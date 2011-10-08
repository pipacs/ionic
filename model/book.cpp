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

Book::Book(): QObject(0), path_(""), lastBookmark_(new Bookmark), loaded_(false), valid_(false), isOpen_(false) {
}

Book::Book(const QString &p, QObject *parent): QObject(parent), lastBookmark_(new Bookmark), loaded_(false), isOpen_(false) {
    setPath(p);
}

Book::~Book() {
    close();
    delete lastBookmark_;
}

void Book::setPath(const QString &p) {
    path_ = "";
    valid_ = false;
    if (p.size()) {
        QFileInfo info(p);
        path_ = info.absoluteFilePath();
        title_ = info.baseName();
        tempFile_.open();
        valid_ = true;
    }
    emit pathChanged();
    emit validChanged();
}

QString Book::path() const {
    return path_;
}

bool Book::isValid() const {
    return valid_;
}

bool Book::open() {
    TRACE;
    qDebug() << path();
    if (!valid_) {
        qDebug() << "Not valid";
        return true;
    }
    if (isOpen_) {
        qDebug() << "Already open";
        return true;
    }
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
    parse();
    setDateOpened(QDateTime::currentDateTime().toUTC());
    isOpen_ = true;
    save();
    emit opened(path());
    return true;
}

void Book::peek() {
    TRACE;
    qDebug() << path();
    if (!valid_) {
        qDebug() << "Not valid";
        return;
    }
    if (isOpen_) {
        qDebug() << "Open already";
        return;
    }
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
    parse();
    save();
    close();
}

void Book::close() {
    Trace t("Book::close");
    clearContent();
    clearParts();
    QDir::setCurrent(QDir::rootPath());
    clearDir(tmpDir());
    isOpen_ = false;
}

QString Book::tmpDir() const {
    QString tmpName = QFileInfo(tempFile_.fileName()).fileName();
    return QDir(QDir::temp().absoluteFilePath("dorian")).absoluteFilePath(tmpName);
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

void Book::parse() {
    TRACE;

    load();

    // Parse OPS file
    QString opsFileName = opsPath();
    qDebug() << "Parsing OPS file" << opsFileName;
    QFile opsFile(opsFileName);
    QXmlSimpleReader reader;
    QXmlInputSource *source = new QXmlInputSource(&opsFile);
    OpsHandler *opsHandler = new OpsHandler(*this);
    XmlErrorHandler *errorHandler = new XmlErrorHandler();
    reader.setContentHandler(opsHandler);
    reader.setErrorHandler(errorHandler);
    reader.parse(source);
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
        coverPath = QDir(rootPath()).absoluteFilePath("cover.jpg");
    }
    setCover(makeCover(coverPath));

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
        reader.parse(source);
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
    emit contentChanged();
    emit sizeChanged();
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
    if (!valid_) {
        return;
    }
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
    setLastBookmark(data["lastpart"].toInt(), data["lastpos"].toReal());
    setCover(data["cover"].value<QImage>());
    if (cover_.isNull()) {
        qWarning() << "Book::load: Cover image is null";
    }
    int size = data["bookmarks"].toInt();
    for (int i = 0; i < size; i++) {
        int part = data[QString("bookmark%1part").arg(i)].toInt();
        qreal pos = data[QString("bookmark%1pos").arg(i)].toReal();
        QString note = data[QString("bookmark%1note").arg(i)].toString();
        bookmarks_.append(new Bookmark(part, pos, note));
    }
    emit bookmarksChanged();
    setDateAdded(data["dateadded"].toDateTime());
    setDateOpened(data["dateopened"].toDateTime());
}

void Book::save() {
    if (!valid_) {
        return;
    }

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
    data["lastpart"] = lastBookmark_->part();
    data["lastpos"] = lastBookmark_->pos();
    data["cover"] = cover_;
    if (cover_.isNull()) {
        qWarning() << "Book::save: Cover image is null";
    }
    data["bookmarks"] = bookmarks_.size();
    for (int i = 0; i < bookmarks_.size(); i++) {
        data[QString("bookmark%1part").arg(i)] = bookmarks_[i]->part();
        data[QString("bookmark%1pos").arg(i)] = bookmarks_[i]->pos();
        data[QString("bookmark%1note").arg(i)] = bookmarks_[i]->note();
    }
    data["dateadded"] = dateAdded_;
    data["dateopened"] = dateOpened_;
    BookDb::instance()->save(path(), data);
}

void Book::setLastBookmark(int part, qreal position) {
    TRACE;
    qDebug() << "Part" << part << "position" << position;
    lastBookmark_->setPart(part);
    lastBookmark_->setPos(position);
    emit lastBookmarkChanged();
}

Bookmark *Book::lastBookmark() {
    load();
    if (!lastBookmark_) {
        qDebug() << "Creating initial last bookmark";
        setLastBookmark(0, 0);
    }
    return lastBookmark_;
}

void Book::addBookmark(int part, qreal position, const QString &note) {
    load();
    bookmarks_.append(new Bookmark(part, position, note));
    qSort(bookmarks_.begin(), bookmarks_.end());
    emit bookmarksChanged();
    save();
}

void Book::deleteBookmark(int index) {
    load();
    bookmarks_.removeAt(index);
    emit bookmarksChanged();
    save();
}

QDeclarativeListProperty<Bookmark> Book::bookmarks() {
    load();
    return QDeclarativeListProperty<Bookmark>(this, bookmarks_);
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

int Book::partFromChapter(int index) {
    TRACE;
    load();
    QString id = chapters_[index];
    QString href = content_[id].href;
    int hashPos = href.indexOf("#");
    if (hashPos != -1) {
        href = href.left(hashPos);
    }

    qDebug() << "Chapter" << index;
    qDebug() << " id" << id;
    qDebug() << " href" << href;

    for (int i = 0; i < parts_.size(); i++) {
        QString partId = parts_[i];
        if (content_[partId].href == href) {
            qDebug() << "Part index for" << href << "is" << i;
            return i;
        }
    }

    qWarning() << "Book::partFromChapter: Could not find part index for" << href;
    return 0;
}

QString Book::fragmentFromChapter(int index) {
    TRACE;
    load();
    QString fragment;
    QString id = chapters_[index];
    QString href = content_[id].href;
    int hashPos = href.indexOf("#");
    if (hashPos != -1) {
        fragment = href.indexOf("#");
    }
    qDebug() << fragment;
    return fragment;
}

QString Book::urlFromPart(int part) {
    TRACE;
    open();
    if (part >= parts_.count()) {
        return QString();
    }
    QString partName = parts_[part];
    QString fullPath = QDir(rootPath_).absoluteFilePath(content_[partName].href);
    QString ret = QUrl::fromLocalFile(fullPath).toString();
    qDebug() << "Return" << ret;
    return ret;
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
    excludedExtensions << ".html" << ".xhtml" << ".xht" << ".htm" << ".html" << ".css" << "*.ttf" << "mimetype";
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
    if (!fileName.isEmpty()) {
        QFileInfo info(fileName);
        if (info.exists() && info.isReadable() && (info.size() < COVER_MAX)) {
            return makeCover(QImage(fileName));
        }
    }
    qDebug() << "Could not read cover file, using default image";
    return makeCover(QImage(":/icons/book.png"));
}

QImage Book::makeCover(const QImage &image) {
    TRACE;
    QImage src = image.scaled(COVER_WIDTH, COVER_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage transparent(COVER_WIDTH, COVER_HEIGHT, QImage::Format_ARGB32);
    transparent.fill(Qt::transparent);
    QPainter p;
    p.begin(&transparent);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawImage((COVER_WIDTH - src.width()) / 2, (COVER_HEIGHT - src.height()) / 2, src);
    p.end();
    return transparent;
}

int Book::partCount() {
    load();
    return parts_.count();
}

QString Book::coverUrl() {
    return QString("image://covers/%1").arg(path());
}

QString Book::dateAddedStr() {
    load();
    return dateAdded_.isValid()? dateAdded_.toString(Qt::SystemLocaleShortDate): QString();
}

QString Book::dateOpenedStr() {
    load();
    return dateOpened_.isValid()? dateOpened_.toString(Qt::SystemLocaleShortDate): QString();
}

QString Book::creatorsString() {
    return creators_.join(", ");
}

QImage Book::cover() {
    load();
    return cover_;
}

void Book::setCover(const QImage &cover) {
    if (cover.isNull()) {
        qWarning() << "Book::setCover: Null cover";
    }
    cover_ = cover;
    emit coverChanged();
}

void Book::addContent(const QString &id, const QString &name, const QString &href) {
    TRACE;
    qDebug() << id << name << href;
    content_[id] = ContentItem(name, href);
    emit contentChanged();
}

QStringList Book::chapterNames() const {
    QStringList ret;
    foreach (QString chapterId, chapters_) {
        ret.append(content_[chapterId].name);
    }
    return ret;
}
