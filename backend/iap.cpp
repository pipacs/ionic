#include <QDebug>

#include "iap.h"

static const char *productIds[] = {
    "849595",
    "849596",
    "849597"
};

Iap::Iap(QObject *parent): QObject(parent) {
    client_ = new IAPClient(this);
    connect(client_, SIGNAL(productDataReceived(int,QString,IAPClient::ProductDataHash)), this, SLOT(onProductDataReceived(int,QString,IAPClient::ProductDataHash)));
    for (size_t i = 0; i < sizeof(productIds) / sizeof(productIds[0]); i++) {
        items_.append(new IapItem(productIds[i], this));
    }
}

void Iap::collectItems() {
    foreach (IapItem *item, items_) {
        int requestId = client_->getProductData(item->id());
        if (requestId < 0) {
            qWarning() << "Iap::collectItems: getProductData" << item->id() << "failed:" << requestId;
        }
    }
}

IapItem *Iap::findItem(const QString &id) {
    foreach (IapItem *item, items_) {
        if (item->id() == id) {
            return item;
        }
    }
    return NULL;
}

void Iap::onProductDataReceived(int requestId, QString status, IAPClient::ProductDataHash data) {
    qDebug() << "Iap::onProductDataReceived: Request" << requestId << "status" << status;
    IapItem *item = findItem(data["id"].toString());
    if (!item) {
        qWarning() << "Iap::onProductDataReceived: Uknown product ID";
        return;
    }
    item->setResult(data["result"].toString());
    item->setInfo(data["info"].toString());
    item->setDescription(data["description"].toString());
    item->setShortDescription(data["shortdescription"].toString());
    item->setPrice(data["price"].toString());
    item->setReady(true); // FIXME: Handle failed request
    emit itemsCollected();
}

void Iap::purchaseItem(IapItem *item) {
    (void)item;
    // FIXME: Implement me
}

QDeclarativeListProperty<IapItem> Iap::items() {
    return QDeclarativeListProperty<IapItem>(this, items_);
}

