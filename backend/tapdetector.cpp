#include <QTimer>

#include "tapdetector.h"

TapDetector::TapDetector(QObject *parent):
    QObject(parent) {
    tapSensor_ = new QTapSensor(this);
    tapSensor_->setProperty("returnDoubleTapEvents", false);
    connect(tapSensor_, SIGNAL(activeChanged()), this, SIGNAL(runningChanged()));
    connect(tapSensor_, SIGNAL(readingChanged()), this, SIGNAL(tap()));
    doubleTapSensor_ = new QTapSensor(this);
    doubleTapSensor_->setProperty("returnDoubleTapEvents", true);
    connect(doubleTapSensor_, SIGNAL(readingChanged()), this, SIGNAL(doubleTap()));
    QTimer::singleShot(0, this, SIGNAL(availableChanged));
}

bool TapDetector::available() {
    return true;
}

bool TapDetector::running() {
    return tapSensor_->isActive();
}

void TapDetector::setRunning(bool v) {
    tapSensor_->setActive(v);
    doubleTapSensor_->setActive(v);
}
