#ifndef IWEBVIEW_H
#define IWEBVIEW_H

#include <QGraphicsProxyWidget>
#include <QWebView>

class IWebSettings;

class IWebView: public QGraphicsProxyWidget {
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString html READ html WRITE setHtml NOTIFY htmlChanged)
    Q_PROPERTY(IWebSettings *settings READ settings)

public:
    explicit IWebView(QGraphicsProxyWidget *parent = 0);
    QUrl url();
    QString html();
    IWebSettings *settings();

signals:
    void loadStarted();
    void loadFailed();
    void loadProgress(int progress);
    void loadFinished(bool success);
    void linkClicked(const QUrl&);
    void urlChanged(const QUrl&);
    void htmlChanged();

public slots:
    void setUrl(const QUrl &url);
    void setHtml(const QString &html);
    Q_INVOKABLE QVariant evaluateJavaScript(const QString &script);

public:
    QWebView *view_;
    IWebSettings *settings_;
};

#endif // IWEBVIEW_H
