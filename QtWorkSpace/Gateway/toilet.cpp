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

void Toilet::on_dial_valueChanged(int value)
{
    qDebug()<<__FUNCTION__<<" value:"<<value;
}

void Toilet::on_pushButton_clicked(bool checked)
{
    stm->setSwitchStat(0,checked?1:0);
}

void Toilet::on_pushButton_2_clicked(bool checked)
{
    stm->setSwitchStat(1,checked?1:0);
}

