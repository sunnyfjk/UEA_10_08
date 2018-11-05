#ifndef MQTTCOMMUNICATIONS_H
#define MQTTCOMMUNICATIONS_H

#include <QObject>
#include <QMqttClient>
#include <stdint.h>

class MqttCommunications : public QObject
{
    Q_OBJECT
public:
    explicit MqttCommunications(QObject *parent = NULL);

signals:
    void changeLedState(uint16_t id,uint8_t power);
    void changeWaterHeater(uint16_t id,uint16_t value);
public slots:
    void sendPm25Value(uint16_t id,uint16_t value);
    void sendLedState(uint16_t id,uint8_t power);
    void sendMotorValue(uint16_t id,uint16_t value);
    void sendAirQuAlity(uint16_t id,uint16_t value);
    void sendWaterHeater(uint16_t id,uint16_t value);

    void connected();
    void disconnected();
    void messageReceived(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());
    void pingResponseReceived();
private:
    QMqttClient *client;
};

#endif // MQTTCOMMUNICATIONS_H
