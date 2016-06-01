#ifndef DATA_H
#define DATA_H

#include <string>
#include <QString>
#include <QDataStream>

class Data
{
public:
    Data(const QString &text, int count, double number);
    void serialize(QDataStream &stream) const;
    void deserialize(QDataStream &stream);
    void printOut();
    QString toString() const;

private:
    QString m_qstring;
    std::string m_string;
    int m_integer;
    double m_double;
};

#endif // DATA_H
