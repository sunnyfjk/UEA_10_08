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

/*
AirQuAlity
Lamp
WaterHeater

*/

void Toilet::sendAirQuAlity(uint16_t id, uint16_t value)
{
    if(id==AirQuAlity_ID){
        ui->AirQuality->display(value);
    }
}

void Toilet::on_Lamp_clicked(bool checked)
{
    stm->setLedStat(Lamp_ID,checked?1:0);

}

void Toilet::on_WaterHeater_clicked(bool checked)
{
    stm->setWaterHeaterValue(WaterHeater_ID,checked?1:0);

}

void Toilet::on_Fan_valueChanged(int value)
{
    ui->FanDisplay->setText(QString("换风扇(%1 r/s)").arg(value));
    stm->setMotorValue(0,value);
}

void Toilet::changeLedState(uint16_t id, uint8_t power)
{
    if(id==Lamp_ID){
        ui->Lamp->setChecked(power?true:false);
        stm->setLedStat(Lamp_ID,power);
    }
}

void Toilet::changeWaterHeater(uint16_t id, uint16_t value)
{
    if(id==WaterHeater_ID){
        ui->WaterHeater->setChecked(value?true:false);
        stm->setWaterHeaterValue(WaterHeater_ID,value);

    }
}
