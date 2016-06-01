#include "data.h"

#include <QDebug>

Data::Data(const QString &text, int count, double number) :
    m_qstring("QString " + text),
    m_string("String " + text.toStdString()),
    m_integer(count),
    m_double(number)
{}

void Data::serialize(QDataStream &stream) const
{
    stream << m_qstring
           << QString(m_string.c_str())
           << static_cast<qint32>(m_integer)
           << m_double;
}

void Data::deserialize(QDataStream &stream)
{
    QString temp;
    stream >> m_qstring;
    stream >> temp;
    stream >> m_integer;
    stream >> m_double;
    m_string = temp.toStdString();
}

void Data::printOut()
{
    qDebug() << m_qstring;
    qDebug() << m_string.c_str();
    qDebug() << m_integer;
    qDebug() << m_double;
}

QString Data::toString() const {
    QString result = m_qstring
            + " " + m_string.c_str()
            + " " + QString::number(m_integer)
            + " " + QString::number(m_double);
    return result;
}
