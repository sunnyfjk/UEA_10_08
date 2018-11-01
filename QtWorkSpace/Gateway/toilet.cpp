#include "toilet.h"
#include "ui_toilet.h"
#include <QDebug>

Toilet::Toilet(QWidget *parent) :
    GeteWayBase(parent),
    ui(new Ui::Toilet)
{
    ui->setupUi(this);
}

Toilet::~Toilet()
{
    delete ui;
}

void Toilet::sendAirQuAlity(uint8_t id, uint16_t value)
{
    if(id==0){
        ui->AirQuality->display(value);
    }
}

void Toilet::on_Lamp_clicked(bool checked)
{
    stm->setLedStat(1,checked?1:0);

}

void Toilet::on_WaterHeater_clicked(bool checked)
{
    stm->setWaterHeaterValue(0,checked?1:0);

}

void Toilet::on_Fan_valueChanged(int value)
{
    ui->FanDisplay->setText(QString("换风扇(%1 r/s)").arg(value));
    stm->setMotorValue(0,value);
}
