#ifndef SENSOR_H
#define SENSOR_H

#include <QString>

class Sensor {
public:
    Sensor(int id, const QString &name, const QString &type, const QString &timestamp, double value);
    int id() const;
    QString name() const;
    QString type() const;
    QString timestamp() const;
    double value() const;

    void setName(const QString &name);
    void setType(const QString &type);
    void setTimestamp(const QString &timestamp);
    void setValue(double value);

private:
    int m_id;
    QString m_name;
    QString m_type;
    QString m_timestamp;
    double m_value;
};

#endif // SENSOR_H
