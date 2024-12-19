// sensor.cpp
#include "sensor.h"

Sensor::Sensor(int id, const QString &name, const QString &type, double value, const QString &timestamp)
    : m_id(id), m_name(name), m_type(type), m_value(value), m_timestamp(timestamp) {}

int Sensor::id() const { return m_id; }
QString Sensor::name() const { return m_name; }
QString Sensor::type() const { return m_type; }
double Sensor::value() const { return m_value; }
QString Sensor::timestamp() const { return m_timestamp; }

void Sensor::setName(const QString &name) { m_name = name; }
void Sensor::setType(const QString &type) { m_type = type; }
void Sensor::setValue(double value) { m_value = value; }
void Sensor::setTimestamp(const QString &timestamp) { m_timestamp = timestamp; }
