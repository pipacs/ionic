#include <QWebFrame>
#include <QDebug>

#include "iwebview.h"
#include "iwebsettings.h"

IWebView::IWebView(QGraphicsProxyWidget *parent):
    QGraphicsProxyWidget(parent) {
    view_ = new QWebView();
    view_->setAttribute(Qt::WA_NoSystemBackground);
    view_->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    view_->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    view_->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(view_->page()->mainFrame(), SIGNAL(contentsSizeChanged(QSize)), this, SIGNAL(contentsSizeChanged(QSize)));
    connect(view_, SIGNAL(linkClicked(QUrl)), this, SIGNAL(linkClicked(QUrl)));
    connect(view_, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
    connect(view_, SIGNAL(loadProgress(int)), this, SIGNAL(loadProgress(int)));
    connect(view_, SIGNAL(loadStarted()), this, SIGNAL(loadStarted()));
    connect(view_, SIGNAL(urlChanged(QUrl)), this, SIGNAL(urlChanged(QUrl)));
    connect(view_, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
    setWidget(view_);
    settings_ = new IWebSettings(view_->settings(), this);
}

IWebView::~IWebView() {
    view_->deleteLater();
}

void IWebView::setUrl(const QUrl &url) {
    view_->setUrl(url);
}

QUrl IWebView::url() {
    return view_->url();
}

void IWebView::setHtml(const QString &html) {
    view_->setHtml(html);
    emit htmlChanged();
}

QString IWebView::html() {
    return view_->page()->mainFrame()->toHtml();
}

IWebSettings *IWebView::settings() {
    return settings_;
}

QVariant IWebView::evaluateJavaScript(const QString &script) {
    return view_->page()->mainFrame()->evaluateJavaScript(script);
}

void IWebView::onLoadFinished(bool result) {
    if (result) {
        emit loadFinished();
    } else {
        emit loadFailed();
    }
}

const QSize IWebView::contentsSize() {
    return view_->page()->mainFrame()->contentsSize();
}

qreal IWebView::prefWidth() {
    return preferredWidth();
}

void IWebView::setPrefWidth(qreal w) {
    setPreferredWidth(w);
    emit prefWidthChanged();
}

qreal IWebView::prefHeight() {
    return preferredHeight();
}

void IWebView::setPrefHeight(qreal h) {
    setPreferredHeight(h);
    emit prefHeightChanged();
}

void IWebView::onSelectionChanged() {
    // Clear selection immediately, effectively disabling all selections
    view_->findText("");
}
