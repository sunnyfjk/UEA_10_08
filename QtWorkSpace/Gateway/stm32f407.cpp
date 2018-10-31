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

quint16 STM32F407::setSwitchStat(uint8_t id, uint8_t power)
{
    union STM32F407Data_t data;
    data.led.head.type=DEVICE_SWITCH_WRITE;
    data.led.id=id;
    data.led.power= !(!power);
    return serialport->write((char *)(&data),sizeof(data));
}

quint16 STM32F407::getSwitchStat(uint8_t id)
{
    union STM32F407Data_t data;
    data.led.head.type=DEVICE_SWITCH_READ;
    data.led.id=id;
    return serialport->write((char *)(&data),sizeof(data));
}

quint16 STM32F407::getPm25Value()
{
    union STM32F407Data_t data;
    data.pm25.head.type=DEVICE_PM25_READ;
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
    qDebug()<<"data.led.power"<<data.led.power;
}
