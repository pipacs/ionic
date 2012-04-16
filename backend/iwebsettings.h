#ifndef IWEBSETTINGS_H
#define IWEBSETTINGS_H

#include <QObject>
#include <QWebSettings>
#include <QDebug>
#include <QString>

class IWebSettings: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString standardFontFamily READ standardFontFamily WRITE setStandardFontFamily NOTIFY standardFontFamilyChanged)
    Q_PROPERTY(int defaultFontSize READ defaultFontSize WRITE setDefaultFontSize NOTIFY defaultFontSizeChanged)
    Q_PROPERTY(int minimumFontSize READ minimumFontSize WRITE setMinimumFontSize NOTIFY minimumFontSizeChanged)
    Q_PROPERTY(bool javaEnabled READ javaEnabled WRITE setJavaEnabled NOTIFY javaEnabledChanged)
    Q_PROPERTY(bool javascriptCanAccessClipboard READ javascriptCanAccessClipboard WRITE setJavascriptCanAccessClipboard NOTIFY javascriptCanAccessClipboardChanged)
    Q_PROPERTY(bool javascriptCanOpenWindows READ javascriptCanOpenWindows WRITE setJavascriptCanOpenWindows NOTIFY javascriptCanOpenWindowsChanged)
    Q_PROPERTY(bool javascriptEnabled READ javascriptEnabled WRITE setJavascriptEnabled NOTIFY javascriptEnabledChanged)
    Q_PROPERTY(bool linksIncludedInFocusChain READ linksIncludedInFocusChain WRITE setLinksIncludedInFocusChain NOTIFY linksIncludedInFocusChainChanged)
    Q_PROPERTY(bool localContentCanAccessRemoteUrls READ localContentCanAccessRemoteUrls WRITE setLocalContentCanAccessRemoteUrls NOTIFY localContentCanAccessRemoteUrlsChanged)
    Q_PROPERTY(bool localStorageDatabaseEnabled READ localStorageDatabaseEnabled WRITE setLocalStorageDatabaseEnabled NOTIFY localStorageDatabaseEnabledChanged)
    Q_PROPERTY(bool offlineStorageDatabaseEnabled READ offlineStorageDatabaseEnabled WRITE setOfflineStorageDatabaseEnabled NOTIFY offlineStorageDatabaseEnabledChanged)
    Q_PROPERTY(bool offlineWebApplicationCacheEnabled READ offlineWebApplicationCacheEnabled WRITE setOfflineWebApplicationCacheEnabled NOTIFY offlineWebApplicationCacheEnabledChanged)
    Q_PROPERTY(bool pluginsEnabled READ pluginsEnabled WRITE setPluginsEnabled NOTIFY pluginsEnabledChanged)

public:
    explicit IWebSettings(QWebSettings *settings = QWebSettings::globalSettings(), QObject *parent = 0): QObject(parent), settings_(settings) {}
    QString standardFontFamily() {return settings_->fontFamily(QWebSettings::StandardFont);}
    int defaultFontSize() {
        qDebug() << "IWebSettings::defaultFontSize:" << settings_->fontSize(QWebSettings::DefaultFontSize);
        return settings_->fontSize(QWebSettings::DefaultFontSize);
    }
    int minimumFontSize() {return settings_->fontSize(QWebSettings::MinimumFontSize);}
    bool javaEnabled() {return false;}
    bool javascriptCanAccessClipboard() {return false;}
    bool javascriptCanOpenWindows() {return false;}
    bool javascriptEnabled() {return false;}
    bool linksIncludedInFocusChain() {return false;}
    bool localContentCanAccessRemoteUrls() {return false;}
    bool localStorageDatabaseEnabled() {return false;}
    bool offlineStorageDatabaseEnabled() {return false;}
    bool offlineWebApplicationCacheEnabled() {return false;}
    bool pluginsEnabled() {return false;}

signals:
    void standardFontFamilyChanged();
    void defaultFontSizeChanged();
    void minimumFontSizeChanged();
    void javaEnabledChanged();
    void javascriptCanAccessClipboardChanged();
    void javascriptCanOpenWindowsChanged();
    void javascriptEnabledChanged();
    void linksIncludedInFocusChainChanged();
    void localContentCanAccessRemoteUrlsChanged();
    void localStorageDatabaseEnabledChanged();
    void offlineStorageDatabaseEnabledChanged();
    void offlineWebApplicationCacheEnabledChanged();
    void pluginsEnabledChanged();

public slots:
    void setStandardFontFamily(const QString &v) {
        settings_->setFontFamily(QWebSettings::StandardFont, v);
        emit standardFontFamilyChanged();
    }
    void setDefaultFontSize(int v) {
        qDebug() << "IWebSettings::setDefaultFontSize:" << v;
        settings_->setFontSize(QWebSettings::DefaultFontSize, v);
        emit defaultFontSizeChanged();
    }
    void setMinimumFontSize(int v) {
        settings_->setFontSize(QWebSettings::MinimumFontSize, v);
        emit minimumFontSizeChanged();
    }
    void setJavaEnabled(bool v) {
        settings_->setAttribute(QWebSettings::JavaEnabled, v);
        emit javaEnabledChanged();
    }
    void setJavascriptCanAccessClipboard(bool v) {
        settings_->setAttribute(QWebSettings::JavascriptCanAccessClipboard, v);
        emit javascriptCanAccessClipboardChanged();
    }
    void setJavascriptCanOpenWindows(bool v) {
        settings_->setAttribute(QWebSettings::JavascriptCanOpenWindows, v);
        emit javascriptCanOpenWindowsChanged();
    }
    void setJavascriptEnabled(bool v) {
        settings_->setAttribute(QWebSettings::JavascriptEnabled, v);
        emit javascriptEnabledChanged();
    }
    void setLinksIncludedInFocusChain(bool v) {
        settings_->setAttribute(QWebSettings::LinksIncludedInFocusChain, v);
        emit linksIncludedInFocusChainChanged();
    }
    void setLocalContentCanAccessRemoteUrls(bool v) {
        settings_->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, v);
        emit localContentCanAccessRemoteUrlsChanged();
    }
    void setLocalStorageDatabaseEnabled(bool v) {
        settings_->setAttribute(QWebSettings::LocalStorageDatabaseEnabled, v);
        emit localStorageDatabaseEnabledChanged();
    }
    void setOfflineStorageDatabaseEnabled(bool v) {
        settings_->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, v);
        emit offlineStorageDatabaseEnabledChanged();
    }
    void setOfflineWebApplicationCacheEnabled(bool v) {
        settings_->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, v);
        emit offlineWebApplicationCacheEnabledChanged();
    }
    void setPluginsEnabled(bool v) {
        settings_->setAttribute(QWebSettings::PluginsEnabled, v);
        emit pluginsEnabledChanged();
    }

public:
    QWebSettings *settings_;
};

#endif // IWEBSETTINGS_H
