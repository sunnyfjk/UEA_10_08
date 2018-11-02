#include "mqttcommunications.h"
#include <QDebug>
MqttCommunications::MqttCommunications(QObject *parent) : QObject(parent)
{
    client = new QMqttClient(this);
    connect(client,SIGNAL(connected()),this,SLOT(connected()));
    connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(client,SIGNAL(messageReceived(QByteArray,QMqttTopicName)),this,SLOT(messageReceived(QByteArray,QMqttTopicName)));
    connect(client,SIGNAL(pingResponseReceived()),this,SLOT(pingResponseReceived()));

    client->setHostname("iot.embsky.com");
    client->setPort(1883);
    client->setUsername("free_irq@163.com");
    client->setPassword("123456");
    client->setClientId("d7675aa9a298fb59b254b13f37d2dc1d");
    client->connectToHost();
}

void MqttCommunications::sendPm25Value(uint16_t id, uint16_t value)
{
    QString topic=QString("eslink/400/694/%1").arg(id);
    QByteArray data=QString("%1").arg(value).toLocal8Bit();
    client->publish(topic,data,2);

}

void MqttCommunications::sendLedState(uint16_t id, uint8_t power)
{
    QString topic=QString("eslink/400/694/%1").arg(id);
    QByteArray data=QString("%1").arg(power).toLocal8Bit();
    client->publish(topic,data,2);
}

void MqttCommunications::sendMotorValue(uint16_t id, uint16_t value)
{
    QString topic=QString("eslink/400/694/%1").arg(id);
    QByteArray data=QString("%1").arg(value).toLocal8Bit();
    client->publish(topic,data,2);
}

void MqttCommunications::sendAirQuAlity(uint16_t id, uint16_t value)
{
    QString topic=QString("eslink/400/694/%1").arg(id);
    QByteArray data=QString("%1").arg(value).toLocal8Bit();
}

void MqttCommunications::sendWaterHeater(uint16_t id, uint16_t value)
{
    QString topic=QString("eslink/400/694/%1").arg(id);
    QByteArray data=QString("%1").arg(value).toLocal8Bit();
    client->publish(topic,data,2);
}

void MqttCommunications::connected()
{
    client->requestPing();
}

void MqttCommunications::disconnected()
{

}

void MqttCommunications::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    QString t=topic.name();
    if(t==QString("eslink/400/694/1374")){
        emit changeLedState(1374,message[0]=='1'?1:0);
    }else if(t==QString("eslink/400/694/1375")){
        emit changeWaterHeater(1375,message[0]=='1'?1:0);
    }
}

void MqttCommunications::pingResponseReceived()
{
    client->subscribe(QString("eslink/400/#"),2);
}
