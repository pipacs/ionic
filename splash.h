#ifndef SPLASH_H
#define SPLASH_H

#include <QSplashScreen>

class Splash: public QSplashScreen {
    Q_OBJECT
public:
    Splash(): QSplashScreen() {setPixmap(QPixmap(":/icons/splash.png"));}
};

#endif // SPLASH_H
