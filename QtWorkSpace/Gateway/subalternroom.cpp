#include "subalternroom.h"
#include "ui_subalternroom.h"

SubalternRoom::SubalternRoom(QWidget *parent) :
    GeteWayBase(parent),
    ui(new Ui::SubalternRoom)
{
    ui->setupUi(this);
}

SubalternRoom::~SubalternRoom()
{
    delete ui;
}
