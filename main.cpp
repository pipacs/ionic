#include <QtGui/QApplication>
#include <QtDeclarative>

#include "qmlapplicationviewer.h"
#include "model/book.h"
#include "trace.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    // Set up application
    QApplication app(argc, argv);

    // Set up tracing
    Settings *settings = Settings::instance();
    Trace::level = (QtMsgType)settings->value("tracelevel",
        (int)QtWarningMsg).toInt();
    Trace::setFileName(settings->value("tracefilename").toString());
    qInstallMsgHandler(Trace::messageHandler);

    // Register QML types
    qmlRegisterType<Book>("com.pipacs.ionic.Book", 1, 0, "Book");

    // Show QML widget with main.qml
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/ionic/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
