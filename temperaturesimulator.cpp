#include "temperaturesimulator.h"

TemperatureSimulator::TemperatureSimulator(QObject *parent) :
    QObject(parent),
    oldValue(20)
{}

QString TemperatureSimulator::getNewValue()
{
    int randomValue = rand() % 5; // 0..4, 5 not included
    int plus = rand() % 2; // range 0..1
    int value(oldValue);
    plus ? value += randomValue : value -= randomValue;
    oldValue = value;
    return QString::number(value);
}

void TemperatureSimulator::generateNewValue()
{
    emit newValue(getNewValue());
}

