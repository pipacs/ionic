#include <QWebFrame>

#include "iwebview.h"
#include "iwebsettings.h"

IWebView::IWebView(QGraphicsProxyWidget *parent):
    QGraphicsProxyWidget(parent) {
    view_ = new QWebView();
    view_->setAttribute(Qt::WA_NoSystemBackground);
    view_->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    view_->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    view_->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    QObject::connect(view_->page()->mainFrame(), SIGNAL(contentsSizeChanged(QSize)), this, SIGNAL(contentSizeChanged()));
    QObject::connect(view_, SIGNAL(linkClicked(QUrl)), this, SIGNAL(linkClicked(QUrl)));
    QObject::connect(view_, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
    QObject::connect(view_, SIGNAL(loadProgress(int)), this, SIGNAL(loadProgress(int)));
    QObject::connect(view_, SIGNAL(urlChanged(QUrl)), this, SIGNAL(urlChanged(QUrl)));
    setWidget(view_);
    settings_ = new IWebSettings(view_->settings(), this);
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
