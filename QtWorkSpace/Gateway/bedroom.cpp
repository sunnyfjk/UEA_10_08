#include "bedroom.h"
#include "ui_bedroom.h"

BedRoom::BedRoom(QWidget *parent) :
    GeteWayBase(parent),
    ui(new Ui::BedRoom)
{
    ui->setupUi(this);
}

BedRoom::~BedRoom()
{
    delete ui;
}
