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
        DEVICE_SWITCH_READ,
        DEVICE_SWITCH_WRITE,
        DEVICE_PM25_READ,
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
    struct DataNodePm25_t{
        struct DateHead_t head;
        uint16_t value;
    };
    union STM32F407Data_t{
        struct DateHead_t head;
        struct DataNodeLed_t led;
        struct DataNodePm25_t pm25;
    };
#pragma pack(pop)

signals:

public slots:
    quint16 setSwitchStat(uint8_t id,uint8_t power);
    quint16 getSwitchStat(uint8_t id);
    quint16 getPm25Value(void);
    bool openSerialPort(QString PortName);
    void readyRead();

private:
    QSerialPort *serialport;
};

#endif // STM32F407_H
