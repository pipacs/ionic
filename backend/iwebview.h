#ifndef IWEBVIEW_H
#define IWEBVIEW_H

#include <QGraphicsProxyWidget>
#include <QWebView>
#include <QSize>
#include <QString>
#include <QUrl>

class IWebView: public QGraphicsProxyWidget {
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString html READ html WRITE setHtml NOTIFY htmlChanged)
    Q_PROPERTY(QSize contentsSize READ contentsSize NOTIFY contentsSizeChanged)
    Q_PROPERTY(QString standardFontFamily READ standardFontFamily WRITE setStandardFontFamily NOTIFY standardFontFamilyChanged)
    Q_PROPERTY(int defaultFontSize READ defaultFontSize WRITE setDefaultFontSize NOTIFY defaultFontSizeChanged)
    Q_PROPERTY(int minimumFontSize READ minimumFontSize WRITE setMinimumFontSize NOTIFY minimumFontSizeChanged)
    Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged)

public:
    explicit IWebView(QGraphicsProxyWidget *parent = 0);
    ~IWebView();
    QUrl url();
    QString html();
    const QSize contentsSize();
    QString standardFontFamily();
    int defaultFontSize();
    int minimumFontSize();
    qreal zoomFactor();

signals:
    void loadStarted();
    void loadFailed();
    void loadFinished();
    void loadProgress(int);
    void linkClicked(QUrl);
    void urlChanged(QUrl);
    void htmlChanged();
    void contentsSizeChanged(QSize);
    void standardFontFamilyChanged();
    void defaultFontSizeChanged();
    void minimumFontSizeChanged();
    void zoomFactorChanged();

public slots:
    void setUrl(const QUrl &url);
    void setHtml(const QString &html);
    Q_INVOKABLE QVariant evaluateJavaScript(const QString &script);
    void onLoadFinished(bool result);
    void onSelectionChanged();
    void setStandardFontFamily(const QString &family);
    void setDefaultFontSize(int size);
    void setMinimumFontSize(int size);
    void setZoomFactor(qreal factor);

public:
    QWebView *view_;
};

#endif // IWEBVIEW_H
