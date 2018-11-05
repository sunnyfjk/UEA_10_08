#ifndef GETEWAYBASE_H
#define GETEWAYBASE_H

#include <QObject>
#include <QWidget>
#include <stm32f407.h>

class GeteWayBase : public QWidget
{
    Q_OBJECT
public:
    enum GeteWayBaseUi_t{
        BedRoom_ui=0,
        CookRoom_ui=1,
        Parlour_ui=2,
        StudyRoom_ui=3,
        SubalternRoom_ui=4,
        Toilet_ui=5,
        HistoryData_ui=6,
    };
    enum DeviceId_t{
        AirQuAlity_ID=1373,
        Lamp_ID=1374,
        WaterHeater_ID=1375,
    };
public:
    explicit GeteWayBase(QWidget *parent = 0);
    virtual void switch_ui( enum GeteWayBaseUi_t id);
    virtual void switch_ui_init(void);
    virtual void setSerialPort( STM32F407 *port);
signals:
    void mqttSendPm25Value(uint16_t id,uint16_t value);
    void mqttSendLedState(uint16_t id,uint8_t power);
    void mqttSendMotorValue(uint16_t id,uint16_t value);
    void mqttSendAirQuAlity(uint16_t id,uint16_t value);
    void mqttSendWaterHeater(uint16_t id,uint16_t value);
public slots:
    virtual void sendPm25Value(uint16_t id,uint16_t value);
    virtual void sendLedState(uint16_t id,uint8_t power);
    virtual void sendMotorValue(uint16_t id,uint16_t value);
    virtual void sendAirQuAlity(uint16_t id,uint16_t value);
    virtual void sendWaterHeater(uint16_t id,uint16_t value);
protected:
    STM32F407 *stm;

};

#endif // GETEWAYBASE_H
