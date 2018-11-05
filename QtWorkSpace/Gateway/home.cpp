#include "home.h"
#include "ui_home.h"

home::home(QWidget *parent) :
    GeteWayBase(parent),
    ui(new Ui::home)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/resource/back.jpg")));
    this->setPalette(palette);


    stm32 = new STM32F407;
    stm32->openSerialPort("/dev/ttySAC1");
    bedroom=new BedRoom(this);
    cookroom=new CookRoom(this);
    parlour= new Parlour(this);
    studyroom=new StudyRoom(this);
    subalternroom=new SubalternRoom(this);
    toilet=new Toilet(this);
    histoydata = new HistoryData(this);
    mqtt = new MqttCommunications;


    /*将设备中的数据存放值数据库*/

    connect(stm32,SIGNAL(sendAirQuAlity(uint16_t,uint16_t)),histoydata,SLOT(sendAirQuAlity(uint16_t,uint16_t)));
    connect(stm32,SIGNAL(sendLedState(uint16_t,uint8_t)),histoydata,SLOT(sendLedState(uint16_t,uint8_t)));
    connect(stm32,SIGNAL(sendMotorValue(uint16_t,uint16_t)),histoydata,SLOT(sendMotorValue(uint16_t,uint16_t)));
    connect(stm32,SIGNAL(sendPm25Value(uint16_t,uint16_t)),histoydata,SLOT(sendPm25Value(uint16_t,uint16_t)));
    connect(stm32,SIGNAL(sendWaterHeater(uint16_t,uint16_t)),histoydata,SLOT(sendWaterHeater(uint16_t,uint16_t)));

    connect(toilet,SIGNAL(mqttSendAirQuAlity(uint16_t,uint16_t)),mqtt,SLOT(sendAirQuAlity(uint16_t,uint16_t)));
    connect(toilet,SIGNAL(mqttSendLedState(uint16_t,uint8_t)),mqtt,SLOT(sendLedState(uint16_t,uint8_t)));
    connect(toilet,SIGNAL(mqttSendMotorValue(uint16_t,uint16_t)),mqtt,SLOT(sendMotorValue(uint16_t,uint16_t)));
    connect(toilet,SIGNAL(mqttSendPm25Value(uint16_t,uint16_t)),mqtt,SLOT(sendPm25Value(uint16_t,uint16_t)));
    connect(toilet,SIGNAL(mqttSendWaterHeater(uint16_t,uint16_t)),mqtt,SLOT(sendWaterHeater(uint16_t,uint16_t)));

    connect(mqtt,SIGNAL(changeLedState(uint16_t,uint8_t)),toilet,SLOT(changeLedState(uint16_t,uint8_t)));
    connect(mqtt,SIGNAL(changeWaterHeater(uint16_t,uint16_t)),toilet,SLOT(changeWaterHeater(uint16_t,uint16_t)));



    toilet->setSerialPort(stm32);


    connect(stm32,SIGNAL(sendAirQuAlity(uint16_t,uint16_t)),toilet,SLOT(sendAirQuAlity(uint16_t,uint16_t)));

    ui->verticalLayout_2->addWidget(bedroom);
    ui->verticalLayout_2->addWidget(cookroom);
    ui->verticalLayout_2->addWidget(parlour);
    ui->verticalLayout_2->addWidget(studyroom);
    ui->verticalLayout_2->addWidget(subalternroom);
    ui->verticalLayout_2->addWidget(toilet);
    ui->verticalLayout_2->addWidget(histoydata);

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
    histoydata->setVisible(false);
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
    case HistoryData_ui:
        histoydata->setVisible(true);
        histoydata->switch_ui_init();
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

void home::on_HistoryData_clicked()
{
    switch_ui(HistoryData_ui);
}
