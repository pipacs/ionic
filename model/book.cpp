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

Book::Book(): QObject(0), mPath(""), loaded(false) {
}

Book::Book(const QString &p, QObject *parent): QObject(parent), loaded(false) {
    setPath(p);
}

Book::~Book() {
    close();
}

void Book::setPath(const QString &p) {
    mPath = "";
    if (p.size()) {
        QFileInfo info(p);
        mPath = info.absoluteFilePath();
        title = info.baseName();
        mTempFile.open();
    }
    emit pathChanged();
}

QString Book::path() {
    return mPath;
}

bool Book::open() {
    TRACE;
    qDebug() << path();
    close();
    clear();
    load();
    if (path().isEmpty()) {
        title = "No book";
        return false;
    }
    if (!extract(QStringList())) {
        return false;
    }
    if (!parse()) {
        return false;
    }
    dateOpened = QDateTime::currentDateTime().toUTC();
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
        title = "No book";
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
    content.clear();
    parts.clear();
    QDir::setCurrent(QDir::rootPath());
    clearDir(tmpDir());
}

QString Book::tmpDir() const {
    QString tmpName = QFileInfo(mTempFile.fileName()).fileName();
    return QDir(QDir::temp().absoluteFilePath("dorian")).
            absoluteFilePath(tmpName);
}

bool Book::extract(const QStringList &excludedExtensions) {
    TRACE;
    bool ret = false;
    QString tmp = tmpDir();
    qDebug() << "Extracting" << mPath << "to" << tmp;

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
    chapters = parts;

    // Load cover image
    QString coverPath;
    QStringList coverKeys;
    coverKeys << "cover-image" << "img-cover-jpeg" << "cover";
    foreach (QString key, coverKeys) {
        if (content.contains(key)) {
            coverPath = QDir(rootPath()).absoluteFilePath(content[key].href);
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
        cover = makeCover(coverPath);
    }

    // If there is an "ncx" item in content, parse it: That's the real table
    // of contents
    QString ncxFileName;
    if (content.contains("ncx")) {
        ncxFileName = content["ncx"].href;
    } else if (content.contains("ncxtoc")) {
        ncxFileName = content["ncxtoc"].href;
    } else if (content.contains("toc")) {
        ncxFileName = content["toc"].href;
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

    // Calculate book part sizes
    size = 0;
    foreach (QString part, parts) {
        QFileInfo info(QDir(rootPath()).absoluteFilePath(content[part].href));
        content[part].size = info.size();
        size += content[part].size;
    }

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
    title = "";
    creators.clear();
    date = "";
    publisher = "";
    datePublished = "";
    subject = "";
    source = "";
    rights = "";
}

void Book::load() {
    if (loaded) {
        return;
    }

    TRACE;
    loaded = true;
    qDebug() << "path" << path();

    QVariantHash data = BookDb::instance()->load(path());
    title = data["title"].toString();
    qDebug() << title;
    creators = data["creators"].toStringList();
    date = data["date"].toString();
    publisher = data["publisher"].toString();
    datePublished = data["datepublished"].toString();
    subject = data["subject"].toString();
    source = data["source"].toString();
    rights = data["rights"].toString();
    mLastBookmark.setPart(data["lastpart"].toInt());
    mLastBookmark.setPos(data["lastpos"].toReal());
    cover = data["cover"].value<QImage>();
    if (cover.isNull()) {
        cover = makeCover(":/icons/book.png");
    }
    int size = data["bookmarks"].toInt();
    for (int i = 0; i < size; i++) {
        int part = data[QString("bookmark%1part").arg(i)].toInt();
        qreal pos = data[QString("bookmark%1pos").arg(i)].toReal();
        QString note = data[QString("bookmark%1note").arg(i)].toString();
        mBookmarks.append(Bookmark(part, pos, note));
    }
    dateAdded = data["dateadded"].toDateTime();
    dateOpened = data["dateopened"].toDateTime();
}

void Book::save() {
    TRACE;

    load();
    QVariantHash data;
    data["title"] = title;
    data["creators"] = creators;
    data["date"] = date;
    data["publisher"] = publisher;
    data["datepublished"] = datePublished;
    data["subject"] = subject;
    data["source"] = source;
    data["rights"] = rights;
    data["lastpart"] = mLastBookmark.part();
    data["lastpos"] = mLastBookmark.pos();
    data["cover"] = cover;
    data["bookmarks"] = mBookmarks.size();
    for (int i = 0; i < mBookmarks.size(); i++) {
        data[QString("bookmark%1part").arg(i)] = mBookmarks[i].part();
        data[QString("bookmark%1pos").arg(i)] = mBookmarks[i].pos();
        data[QString("bookmark%1note").arg(i)] = mBookmarks[i].note();
    }
    data["dateadded"] = dateAdded;
    data["dateopened"] = dateOpened;
    BookDb::instance()->save(path(), data);
}

void Book::setLastBookmark(int part, qreal position, bool fast) {
    TRACE;
    qDebug() << "Part" << part << "position" << position << "fast?" << fast;
    if (!fast) {
        load();
    }
    mLastBookmark.setPart(part);
    mLastBookmark.setPos(position);
    if (!fast) {
        save();
    }
}

Bookmark Book::lastBookmark() {
    load();
    return mLastBookmark;
}

void Book::addBookmark(int part, qreal position, const QString &note) {
    load();
    mBookmarks.append(Bookmark(part, position, note));
    qSort(mBookmarks.begin(), mBookmarks.end());
    save();
}

void Book::setBookmarkNote(int index, const QString &note) {
    load();
    if (index >= 0 && index < mBookmarks.length()) {
        mBookmarks[index].setNote(note);
    }
    save();

}

void Book::deleteBookmark(int index) {
    load();
    mBookmarks.removeAt(index);
    save();
}

QList<Bookmark> Book::bookmarks() {
    load();
    return mBookmarks;
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
        mRootPath = QFileInfo(ret).absoluteDir().absolutePath();
        qDebug() << "OSP path" << ret << "\nRoot dir" << mRootPath;
    }
    delete errorHandler;
    delete containerHandler;
    delete source;
    return ret;
}

QString Book::rootPath() {
    load();
    return mRootPath;
}

QString Book::name() {
    load();
    if (title.size()) {
        QString ret = title;
        if (creators.length()) {
            ret += "\nBy " + creators.join(", ");
        }
        return ret;
    }
    return path();
}

QString Book::shortName() {
    load();
    return (title.isEmpty())? QFileInfo(path()).baseName(): title;
}

QImage Book::coverImage() {
    load();
    return cover;
}

int Book::chapterFromPart(int index) {
    TRACE;
    load();
    int ret = -1;

    QString partId = parts[index];
    QString partHref = content[partId].href;

    for (int i = 0; i < chapters.size(); i++) {
        QString id = chapters[i];
        QString href = content[id].href;
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
    QString id = chapters[index];
    QString href = content[id].href;
    int hashPos = href.indexOf("#");
    if (hashPos != -1) {
        fragment = href.mid(hashPos);
        href = href.left(hashPos);
    }

    qDebug() << "Chapter" << index;
    qDebug() << " id" << id;
    qDebug() << " href" << href;
    qDebug() << " fragment" << fragment;

    for (int i = 0; i < parts.size(); i++) {
        QString partId = parts[i];
        if (content[partId].href == href) {
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
        key = parts[i];
        partSize += content[key].size;
    }
    key = parts[part];
    partSize += content[key].size * position;
    return partSize / (qreal)size;
}

bool Book::extractMetaData() {
    QStringList excludedExtensions;
    excludedExtensions << ".html" << ".xhtml" << ".xht" << ".htm" << ".gif"
            << ".css" << "*.ttf" << "mimetype";
    return extract(excludedExtensions);
}

void Book::upgrade() {
    TRACE;

    // Load book from old database (QSettings)
    QSettings settings;
    QString key = "book/" + path() + "/";
    title = settings.value(key + "title").toString();
    qDebug() << title;
    creators = settings.value(key + "creators").toStringList();
    date = settings.value(key + "date").toString();
    publisher = settings.value(key + "publisher").toString();
    datePublished = settings.value(key + "datepublished").toString();
    subject = settings.value(key + "subject").toString();
    source = settings.value(key + "source").toString();
    rights = settings.value(key + "rights").toString();
    mLastBookmark.setPart(settings.value(key + "lastpart").toInt());
    mLastBookmark.setPos(settings.value(key + "lastpos").toReal());
    cover = settings.value(key + "cover").value<QImage>();
    if (cover.isNull()) {
        cover = makeCover(":/icons/book.png");
    } else {
        cover = makeCover(QPixmap::fromImage(cover));
    }
    int size = settings.value(key + "bookmarks").toInt();
    for (int i = 0; i < size; i++) {
        int part = settings.value(key + "bookmark" + QString::number(i) +
                                     "/part").toInt();
        qreal pos = settings.value(key + "bookmark" + QString::number(i) +
                                   "/pos").toReal();
        qDebug() << QString("Bookmark %1 at part %2, %3").
                arg(i).arg(part).arg(pos);
        mBookmarks.append(Bookmark(part, pos));
    }

    // Remove QSettings
    settings.remove("book/" + path());

    // Save book to new database
    save();
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
