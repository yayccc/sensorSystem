#include "sensor.h"

Sensor::Sensor(int id, const QString &name, const QString &type, const QString &timestamp, double value)
    : m_id(id), m_name(name), m_type(type), m_timestamp(timestamp), m_value(value) {}

int Sensor::id() const {
    return m_id;
}

QString Sensor::name() const {
    return m_name;
}

QString Sensor::type() const {
    return m_type;
}

QString Sensor::timestamp() const {
    return m_timestamp;
}

double Sensor::value() const {
    return m_value;
}

void Sensor::setName(const QString &name) {
    m_name = name;
}

void Sensor::setType(const QString &type) {
    m_type = type;
}

void Sensor::setTimestamp(const QString &timestamp) {
    m_timestamp = timestamp;
}

void Sensor::setValue(double value) {
    m_value = value;
}
