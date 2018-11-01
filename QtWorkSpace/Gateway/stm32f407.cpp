#include "stm32f407.h"
#include <QDebug>

STM32F407::STM32F407(QObject *parent) : QObject(parent)
{
    serialport = new QSerialPort;
    connect(serialport,SIGNAL(readyRead()),this,SLOT(readyRead()));
    serialport->setBaudRate(QSerialPort::Baud115200);
    serialport->setFlowControl(QSerialPort::NoFlowControl);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setParity(QSerialPort::NoParity);
    serialport->setStopBits(QSerialPort::OneStop );
    serialport->setReadBufferSize(sizeof(union STM32F407Data_t));
}

quint16 STM32F407::setLedStat(uint8_t id, uint8_t power)
{
    union STM32F407Data_t data;
    data.led.head.type=DEVICE_LED_WRITE;
    data.led.id=id;
    data.led.power= !(!power);
    return serialport->write((char *)(&data),sizeof(data));
}

quint16 STM32F407::getLedStat(uint8_t id)
{
    union STM32F407Data_t data;
    data.led.head.type=DEVICE_LED_READ;
    data.led.id=id;
    return serialport->write((char *)(&data),sizeof(data));
}

quint16 STM32F407::getPm25Value(uint8_t id)
{
    union STM32F407Data_t data;
    data.pm25.head.type=DEVICE_PM25_READ;
    data.pm25.id=id;
    return serialport->write((char *)(&data),sizeof(data));
}

quint16 STM32F407::setMotorValue(uint8_t id, uint16_t value)
{
    union STM32F407Data_t data;
    data.motor.head.type=DEVICE_MOTOR_WRITE;
    data.motor.id=id;
    data.motor.value=value;
    return serialport->write((char *)(&data),sizeof(data));
}

quint16 STM32F407::getMotorValue(uint8_t id)
{
    union STM32F407Data_t data;
    data.motor.head.type=DEVICE_MOTOR_READ;
    data.motor.id=id;
    return serialport->write((char *)(&data),sizeof(data));
}

quint16 STM32F407::setWaterHeaterValue(uint8_t id, uint8_t power)
{
    union STM32F407Data_t data;
    data.waterheater.head.type=DEVICE_WATER_HEATER_WRITE;
    data.waterheater.id=id;
    data.waterheater.power= !(!power);
    return serialport->write((char *)(&data),sizeof(data));
}

quint16 STM32F407::getWaterHeaterValue(uint8_t id)
{
    union STM32F407Data_t data;
    data.waterheater.head.type=DEVICE_MOTOR_READ;
    data.waterheater.id=id;
    return serialport->write((char *)(&data),sizeof(data));
}

bool STM32F407::openSerialPort(QString PortName)
{
    serialport->setPortName(PortName);
    return serialport->open(QIODevice::ReadWrite);
}

void STM32F407::readyRead()
{
    union STM32F407Data_t data;
    serialport->read((char *)(&data),sizeof(data));
    switch (data.head.type) {
    case DEVICE_LED_READ:
        emit sendSwitchState(data.led.id,data.led.power);
        qDebug()<<__FUNCTION__<<"data.led.id:"<<data.led.id<<"data.led.power:"<<data.led.power;
        break;
    case DEVICE_PM25_READ:
        emit sendPm25Value(data.pm25.id,data.pm25.value);
        qDebug()<<__FUNCTION__<<"data.pm25.id:"<<data.pm25.id<<"data.pm25.power:"<<data.pm25.value;
        break;
    case DEVICE_MOTOR_READ:
        emit sendMotorValue(data.motor.id,data.motor.value);
        qDebug()<<__FUNCTION__<<"data.motor.id:"<<data.motor.id<<"data.motor.power:"<<data.motor.value;
        break;
    case DEVICE_AIR_QUALITY_READ:
        emit sendAirQuAlity(data.air.id,data.air.value);
        qDebug()<<__FUNCTION__<<"data.air.id:"<<data.air.id<<"data.air.power:"<<data.air.value;
        break;
    case DEVICE_WATER_HEATER_READ:
        emit sendWaterHeater(data.waterheater.id,data.waterheater.power);
        qDebug()<<__FUNCTION__<<"data.waterheater.id:"<<data.waterheater.id<<"data.waterheater.power:"<<data.waterheater.power;

        break;
    }
}
