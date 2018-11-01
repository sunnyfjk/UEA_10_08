#ifndef STM32F407_H
#define STM32F407_H

#include <QObject>
#include <QSerialPort>

class STM32F407 : public QObject
{
    Q_OBJECT
public:
    explicit STM32F407(QObject *parent = 0);

    enum STM32F407DataType_t{
        DEVICE_LED_READ,
        DEVICE_LED_WRITE,
        DEVICE_PM25_READ,
        DEVICE_MOTOR_WRITE,
        DEVICE_MOTOR_READ,
        DEVICE_AIR_QUALITY_READ,
        DEVICE_WATER_HEATER_READ,
        DEVICE_WATER_HEATER_WRITE,
    };

#pragma pack(push,1)
    struct DateHead_t{
        uint8_t type;
    };
    struct DataNodeLed_t{
        struct DateHead_t head;
        uint8_t id;
        uint8_t power;
    };
    struct DataNodeWaterHeater_t{
        struct DateHead_t head;
        uint8_t id;
        uint8_t power;
    };
    struct DataNodeMotor_t{
        struct DateHead_t head;
        uint8_t id;
        uint8_t value;
    };
    struct DataNodePm25_t{
        struct DateHead_t head;
        uint8_t id;
        uint16_t value;
    };
    struct DataAirQuAlity_t{
        struct DateHead_t head;
        uint8_t id;
        uint16_t value;
    };
    union STM32F407Data_t{
        struct DateHead_t head;
        struct DataNodeLed_t led;
        struct DataNodePm25_t pm25;
        struct DataNodeMotor_t motor;
        struct DataAirQuAlity_t air;
        struct DataNodeWaterHeater_t waterheater;
    };
#pragma pack(pop)

signals:
    void sendPm25Value(uint8_t id,uint16_t value);
    void sendSwitchState(uint8_t id,uint8_t power);
    void sendMotorValue(uint8_t id,uint16_t value);
    void sendAirQuAlity(uint8_t id,uint16_t value);
    void sendWaterHeater(uint8_t id,uint16_t value);

public slots:
    quint16 setLedStat(uint8_t id,uint8_t power);
    quint16 getLedStat(uint8_t id);
    quint16 getPm25Value(uint8_t id);
    quint16 setMotorValue(uint8_t id,uint16_t value);
    quint16 getMotorValue(uint8_t id);
    quint16 setWaterHeaterValue(uint8_t id,uint8_t power);
    quint16 getWaterHeaterValue(uint8_t id);

    bool openSerialPort(QString PortName);
    void readyRead();

private:
    QSerialPort *serialport;
};

#endif // STM32F407_H
