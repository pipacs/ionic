#ifndef TAPDETECTOR_H
#define TAPDETECTOR_H

#include <QObject>
#include <QTapSensor>

QTM_USE_NAMESPACE

class TapDetector: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool available READ available NOTIFY availableChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

public:
    explicit TapDetector(QObject *parent = 0);
    bool available();
    bool running();

signals:
    void availableChanged();
    void runningChanged();
    void tap();
    void doubleTap();

public slots:
    void setRunning(bool v);

public:
    QTapSensor *tapSensor_;
    QTapSensor *doubleTapSensor_;
};

#endif // TAPDETECTOR_H
