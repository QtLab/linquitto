#include "temperaturesimulator.h"
#include <QDebug>

TemperatureSimulator::TemperatureSimulator(QObject *parent) :
    QObject(parent),
    oldValue(2000)
{}

QString TemperatureSimulator::getNewValue()
{
    int randomValue = rand() % 40; // 0..39, 40 not included
    int plus = rand() % 2; // range 0..1
    int value(oldValue);
    plus ? value += randomValue : value -= randomValue;
    oldValue = value;
    double doubleValue = value / 100.0;
    qDebug() << "double =" << doubleValue;
    return QString::number(doubleValue, 'f', 1);
}

void TemperatureSimulator::generateNewValue()
{
    emit newValue(getNewValue());
}

