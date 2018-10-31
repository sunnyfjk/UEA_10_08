#include "studyroom.h"
#include "ui_studyroom.h"

StudyRoom::StudyRoom(QWidget *parent) :
    GeteWayBase(parent),
    ui(new Ui::StudyRoom)
{
    ui->setupUi(this);
}

StudyRoom::~StudyRoom()
{
    delete ui;
}
