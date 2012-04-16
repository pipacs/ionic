#ifndef IWEBVIEW_H
#define IWEBVIEW_H

#include <QGraphicsProxyWidget>
#include <QWebView>
#include <QSize>
#include <QString>
#include <QUrl>

class IWebSettings;

class IWebView: public QGraphicsProxyWidget {
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString html READ html WRITE setHtml NOTIFY htmlChanged)
    Q_PROPERTY(IWebSettings *settings READ settings)
    Q_PROPERTY(QSize contentsSize READ contentsSize NOTIFY contentsSizeChanged)
    Q_PROPERTY(qreal preferredWidth READ prefWidth WRITE setPrefWidth NOTIFY prefWidthChanged)
    Q_PROPERTY(qreal preferredHeight READ prefHeight WRITE setPrefHeight NOTIFY prefHeightChanged)

public:
    explicit IWebView(QGraphicsProxyWidget *parent = 0);
    ~IWebView();
    QUrl url();
    QString html();
    IWebSettings *settings();
    const QSize contentsSize();
    qreal prefWidth();
    qreal prefHeight();

signals:
    void loadStarted();
    void loadFailed();
    void loadFinished();
    void loadProgress(int);
    void linkClicked(QUrl);
    void urlChanged(QUrl);
    void htmlChanged();
    void contentsSizeChanged(QSize);
    void prefWidthChanged();
    void prefHeightChanged();

public slots:
    void setUrl(const QUrl &url);
    void setHtml(const QString &html);
    Q_INVOKABLE QVariant evaluateJavaScript(const QString &script);
    void onLoadFinished(bool result);
    void setPrefWidth(qreal w);
    void setPrefHeight(qreal h);
    void onSelectionChanged();

public:
    QWebView *view_;
    IWebSettings *settings_;
};

#endif // IWEBVIEW_H
