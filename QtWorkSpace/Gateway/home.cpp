#include "home.h"
#include "ui_home.h"

home::home(QWidget *parent) :
    GeteWayBase(parent),
    ui(new Ui::home)
{
    ui->setupUi(this);
    stm32 = new STM32F407;
    stm32->openSerialPort("/dev/ttyUSB0");
    bedroom=new BedRoom(ui->widget);
    cookroom=new CookRoom(ui->widget);
    parlour= new Parlour(ui->widget);
    studyroom=new StudyRoom(ui->widget);
    subalternroom=new SubalternRoom(ui->widget);
    toilet=new Toilet(ui->widget);
    toilet->setSerialPort(stm32);


     connect(stm32,SIGNAL(sendAirQuAlity(uint8_t,uint16_t)),toilet,SLOT(sendAirQuAlity(uint8_t,uint16_t)));

    ui->verticalLayout_2->addWidget(bedroom);
    ui->verticalLayout_2->addWidget(cookroom);
    ui->verticalLayout_2->addWidget(parlour);
    ui->verticalLayout_2->addWidget(studyroom);
    ui->verticalLayout_2->addWidget(subalternroom);
    ui->verticalLayout_2->addWidget(toilet);

    switch_ui(BedRoom_ui);
}

home::~home()
{
    delete ui;
}

void home::switch_ui(GeteWayBase::GeteWayBaseUi_t id)
{
    bedroom->setVisible(false);
    cookroom->setVisible(false);
    parlour->setVisible(false);
    studyroom->setVisible(false);
    subalternroom->setVisible(false);
    toilet->setVisible(false);
    switch (id) {
    case GeteWayBase::BedRoom_ui:
        bedroom->setVisible(true);
        break;
    case GeteWayBase::CookRoom_ui:
        cookroom->setVisible(true);
        break;
    case GeteWayBase::Parlour_ui:
        parlour->setVisible(true);
        break;
    case GeteWayBase::StudyRoom_ui:
        studyroom->setVisible(true);
        break;
    case GeteWayBase::SubalternRoom_ui:
        subalternroom->setVisible(true);
        break;
    case GeteWayBase::Toilet_ui:
        toilet->setVisible(true);
        break;
    default:
        break;
    };
}

void home::on_Parlour_clicked()
{
    switch_ui(Parlour_ui);
}

void home::on_StudyRoom_clicked()
{
    switch_ui(StudyRoom_ui);
}

void home::on_BedRoom_clicked()
{
    switch_ui(BedRoom_ui);
}

void home::on_SubalternRoom_clicked()
{
    switch_ui(SubalternRoom_ui);
}

void home::on_CookRoom_clicked()
{
    switch_ui(CookRoom_ui);
}

void home::on_Toilet_clicked()
{
    switch_ui(Toilet_ui);
}
