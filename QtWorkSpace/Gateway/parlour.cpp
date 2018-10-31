#include "parlour.h"
#include "ui_parlour.h"

Parlour::Parlour(QWidget *parent) :
    GeteWayBase(parent),
    ui(new Ui::Parlour)
{
    ui->setupUi(this);
}

Parlour::~Parlour()
{
    delete ui;
}
