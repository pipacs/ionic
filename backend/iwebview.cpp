#include <QWebFrame>
#include <QDebug>
#include <QWebSettings>

#include "iwebview.h"

IWebView::IWebView(QGraphicsProxyWidget *parent):
    QGraphicsProxyWidget(parent) {
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::JavaEnabled, false);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::LinksIncludedInFocusChain, false);
    settings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, false);
    settings->setAttribute(QWebSettings::LocalStorageEnabled, false);
    settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, false);
    settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, false);
    settings->setAttribute(QWebSettings::PluginsEnabled, false);

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

void IWebView::onSelectionChanged() {
    // Clear selection immediately, effectively disabling all selections
    view_->findText("");
}

QString IWebView::standardFontFamily() {
    return view_->settings()->fontFamily(QWebSettings::StandardFont);
}

int IWebView::defaultFontSize() {
    return view_->settings()->fontSize(QWebSettings::DefaultFontSize);
}

int IWebView::minimumFontSize() {
    return view_->settings()->fontSize(QWebSettings::MinimumFontSize);
}

void IWebView::setStandardFontFamily(const QString &family) {
    view_->settings()->setFontFamily(QWebSettings::StandardFont, family);
    emit standardFontFamilyChanged();
}

void IWebView::setDefaultFontSize(int size) {
    view_->settings()->setFontSize(QWebSettings::DefaultFontSize, size);
    emit defaultFontSizeChanged();
}

void IWebView::setMinimumFontSize(int size) {
    view_->settings()->setFontSize(QWebSettings::MinimumFontSize, size);
    emit minimumFontSizeChanged();
}

qreal IWebView::zoomFactor() {
    return view_->page()->mainFrame()->zoomFactor();
}

void IWebView::setZoomFactor(qreal factor) {
    view_->page()->mainFrame()->setZoomFactor(factor);
    emit zoomFactorChanged();
    qDebug() << "IWebView::setZoomFactor: New contents size is " << view_->page()->mainFrame()->contentsSize();
}
