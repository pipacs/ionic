#include <QDebug>

#include "iap.h"

struct {
    const char *id;
    const char *description;
    const char *price;
} products[] = {
    {"849595", "Donate 1EUR", "1 EUR"},
    {"849596", "Donate 5EUR", "5 EUR"},
    {"849597", "Donate 10EUR", "10 EUR"}
};

Iap::Iap(QObject *parent): QObject(parent) {
    client_ = new IAPClient(this);
    connect(client_, SIGNAL(productDataReceived(int,QString,IAPClient::ProductDataHash)), this, SLOT(onProductDataReceived(int,QString,IAPClient::ProductDataHash)));
    connect(client_, SIGNAL(purchaseCompleted(int,QString,QString)), this, SLOT(onPurchaseCompleted(int,QString,QString)));
    connect(client_, SIGNAL(purchaseFlowFinished(int)), this, SLOT(onPurchaseFlowFinished(int)));
    for (size_t i = 0; i < sizeof(products) / sizeof(products[0]); i++) {
        items_.append(new IapItem(products[i].id, products[i].description, products[i].price, this));
    }
}

void Iap::updateItems() {
    foreach (IapItem *item, items_) {
        int requestId = client_->getProductData(item->itemId());
        if (requestId < 0) {
            qWarning() << "Iap::collectItems: getProductData" << item->itemId() << "failed:" << requestId;
        }
    }
}

IapItem *Iap::findItem(const QString &id) {
    foreach (IapItem *item, items_) {
        if (item->itemId() == id) {
            return item;
        }
    }
    return NULL;
}

IapItem *Iap::findItemByPurchaseRequest(int request) {
    foreach (IapItem *item, items_) {
        if (item->purchaseRequest() == request) {
            return item;
        }
    }
    return NULL;
}

void Iap::onProductDataReceived(int requestId, QString status, IAPClient::ProductDataHash data) {
    qDebug() << "Iap::onProductDataReceived: Request" << requestId << "status" << status << "ID" << data["id"].toString();
    IapItem *item = findItem(data["id"].toString());
    if (item && (status == "OK")) {
        item->setResult(data["result"].toString());
        item->setInfo(data["info"].toString());
        item->setDescription(data["description"].toString());
        item->setShortDescription(data["shortdescription"].toString());
        item->setPrice(data["price"].toString());
        item->setReady(true);
    }
    emit itemsUpdated();
}

void Iap::purchase(const QString &id) {
    IapItem *item = findItem(id);
    if (!item) {
        qWarning() << "Iap::purchase: Item" << id << "not found";
        return;
    }
    int requestId = client_->purchaseProduct(id, IAPClient::NoForcedRestoration);
    if (requestId < 0) {
        emit purchaseFlowFinished();
    }
    item->setPurchaseRequest(requestId);
}

QDeclarativeListProperty<IapItem> Iap::items() {
    return QDeclarativeListProperty<IapItem>(this, items_);
}

void Iap::onPurchaseCompleted(int requestId, QString status, QString purchaseTicket) {
    (void)purchaseTicket;
    qDebug() << "Iap::onPurchaseCompleted: Request" << requestId << "status" << status;
    if (requestId < 0) {
        qWarning() << "Iap::onPurchaseCompleted: Invalid purchase request ID";
        return;
    }
    if (status != "OK") {
        qWarning() << "Iap::onPurchaseCompleted: Status" << status;
        return;
    }
    IapItem *item = findItemByPurchaseRequest(requestId);
    if (!item) {
        qWarning() << "Iap::onPurchaseCompleted: Unknown purchase request ID";
        return;
    }
    item->setPurchaseRequest(-1);
    item->incrementPurchaseCount();
}

void Iap::onPurchaseFlowFinished(int requestId) {
    qDebug() << "Iap::onPurchaseFlowFinished: Request" << requestId;
    if (requestId < 0) {
        qWarning() << "Iap::onPurchaseFlowFinished: Invalid purchase request ID";
        return;
    }
    IapItem *item = findItemByPurchaseRequest(requestId);
    if (item) {
        item->setPurchaseRequest(-1);
    }
    emit purchaseFlowFinished();
}
