#include "cookroom.h"
#include "ui_cookroom.h"

CookRoom::CookRoom(QWidget *parent) :
    GeteWayBase(parent),
    ui(new Ui::CookRoom)
{
    ui->setupUi(this);
}

CookRoom::~CookRoom()
{
    delete ui;
}
