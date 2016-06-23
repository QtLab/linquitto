#ifndef TEMPERATURESIMULATOR_H
#define TEMPERATURESIMULATOR_H

#include <QObject>

class TemperatureSimulator : public QObject
{
    Q_OBJECT
public:
    explicit TemperatureSimulator(QObject *parent = 0);

    QString getNewValue();

signals:
    void newValue(const QString &value);

public slots:
    void generateNewValue();


private:
    int oldValue;
};

#endif // TEMPERATURESIMULATOR_H
