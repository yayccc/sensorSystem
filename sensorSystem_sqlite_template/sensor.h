// sensor.h
#ifndef SENSOR_H
#define SENSOR_H

#include <QString>

class Sensor {
public:
    Sensor(int id, const QString &name, const QString &type, double value, const QString &timestamp);
    int id() const;
    QString name() const;
    QString type() const;
    double value() const;
    QString timestamp() const;

    void setName(const QString &name);
    void setType(const QString &type);
    void setValue(double value);
    void setTimestamp(const QString &timestamp);

private:
    int m_id;
    QString m_name;
    QString m_type;
    double m_value;
    QString m_timestamp;
};

#endif // SENSOR_H
