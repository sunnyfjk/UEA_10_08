#ifndef TOILET_H
#define TOILET_H

#include <QWidget>
#include <getewaybase.h>

namespace Ui {
class Toilet;
}

class Toilet : public GeteWayBase
{
    Q_OBJECT

public:
    explicit Toilet(QWidget *parent = 0);
    ~Toilet();
public slots:
    void sendAirQuAlity(uint16_t id,uint16_t value);

private slots:
    void on_Lamp_clicked(bool checked);

    void on_WaterHeater_clicked(bool checked);

    void on_Fan_valueChanged(int value);

    void changeLedState(uint16_t id,uint8_t power);

    void changeWaterHeater(uint16_t id,uint16_t value);

private:
    Ui::Toilet *ui;
};

#endif // TOILET_H
