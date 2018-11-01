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
public:
    explicit GeteWayBase(QWidget *parent = 0);
    virtual void switch_ui( enum GeteWayBaseUi_t id);
    virtual void switch_ui_init(void);
    virtual void setSerialPort( STM32F407 *port);
signals:

public slots:

protected:
    STM32F407 *stm;

};

#endif // GETEWAYBASE_H
