#ifndef IAP_H
#define IAP_H

#include <QObject>
#include <QDeclarativeListProperty>
#include <QString>
#include <QVariantHash>

#include "iapclient.h"

/// Purchasable item.
class IapItem: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id NOTIFY itemChanged)
    Q_PROPERTY(QString result READ result NOTIFY itemChanged)
    Q_PROPERTY(QString info READ info NOTIFY itemChanged)
    Q_PROPERTY(QString description READ description NOTIFY itemChanged)
    Q_PROPERTY(QString shortDescription READ shortDescription NOTIFY itemChanged)
    Q_PROPERTY(QString price READ price NOTIFY itemChanged)
    Q_PROPERTY(bool ready READ ready NOTIFY itemChanged)

public:
    IapItem(const QString &id = QString("42"), QObject *parent = 0): QObject(parent) {
        data_["id"] = data_["description"] = data_["shortdescription"] = data_["info"] = id;
    }
    QString id() {return data_.value("id").toString();}
    QString result() {return data_.value("result").toString();}
    QString info() {return data_.value("info").toString();}
    QString description() {return data_.value("description").toString();}
    QString shortDescription() {return data_.value("shortdescription").toString();}
    QString price() {return data_.value("price").toString();}
    bool ready() {return data_.value("ready").toBool();}

signals:
    void itemChanged();

public slots:
    void setResult(const QString &v) {data_["result"] = v; emit itemChanged();}
    void setInfo(const QString &v) {data_["info"] = v; emit itemChanged();}
    void setDescription(const QString &v) {data_["description"] = v; emit itemChanged();}
    void setShortDescription(const QString &v) {data_["shortDescription"] = v; emit itemChanged();}
    void setPrice(const QString &v) {data_["price"] = v; emit itemChanged();}
    void setReady(bool v) {data_["ready"] = v; emit itemChanged();}

public:
    QVariantHash data_;
};

/// In-application purchase.
class Iap: public QObject {
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<IapItem> items READ items NOTIFY itemsCollected)

public:
    explicit Iap(QObject *parent = 0);
    IapItem *findItem(const QString &id);

signals:
    void itemsCollected();
    void purchaseCompleted();
    void purchaseFlowFinished();

public slots:
    Q_INVOKABLE void collectItems();
    Q_INVOKABLE void purchaseItem(IapItem *item);
    QDeclarativeListProperty<IapItem> items();
    void onProductDataReceived(int requestId, QString status, IAPClient::ProductDataHash data);

public:
    QList<IapItem *> items_;
    IAPClient *client_;
};

#endif // IAP_H
