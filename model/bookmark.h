#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QObject>

/**
Bookmark: A location in the book identified by a part (volume) index and
a relative position in volume.
*/
class Bookmark: public QObject {

    Q_OBJECT
    Q_PROPERTY(int part READ part WRITE setPart NOTIFY partChanged)
    Q_PROPERTY(qreal pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)

public:
    Bookmark(): QObject(0), part_(0), pos_(0), note_("") {
    }

    Bookmark(int part, qreal pos, const QString &note = QString(),
        QObject *parent = 0): QObject(parent), part_(part), pos_(pos),
        note_(note) {
    }

    Bookmark(const Bookmark &other): QObject(), part_(other.part_),
        pos_(other.pos_), note_(other.note_) {
    }

    bool operator<(const Bookmark &other) const {
        return (part_ == other.part_)?
            (pos_ < other.pos_): (part_ < other.part_);
    }

    Bookmark& operator=(const Bookmark &other) {
        if (this != &other) {
            pos_ = other.pos_;
            part_ = other.part_;
            note_ = other.note_;
        }
        return *this;
    }

    int part() const {return part_;}
    void setPart(int p) {part_ = p; emit partChanged();}
    qreal pos() const {return pos_;}
    void setPos(const qreal p) {pos_ = p; emit posChanged();}
    QString note() const {return note_;}
    void setNote(const QString &n) {note_ = n; emit noteChanged();}

signals:
    void partChanged();
    void posChanged();
    void noteChanged();

private:
    int part_;
    qreal pos_;
    QString note_;
};

#endif // BOOKMARK_H
