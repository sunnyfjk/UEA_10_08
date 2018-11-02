#include "historydata.h"
#include "ui_historydata.h"
#include <QDebug>

HistoryData::HistoryData(QWidget *parent) :
    GeteWayBase(parent),
    ui(new Ui::HistoryData)
{
    ui->setupUi(this);
    /*让显示的数据平均分配空间*/
    ui->DisplaySqlData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->DisplaySqlData->verticalHeader()->setVisible(false);
    ui->DisplaySqlData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->DisplaySqlData->setSelectionBehavior(QAbstractItemView::SelectRows);

    mysql = new QSqlDatabase();
    *mysql=QSqlDatabase::addDatabase("QMYSQL");
    mysql->setHostName("127.0.0.1");
    mysql->setPort(3306);
    mysql->setDatabaseName("mqtt");
    mysql->setUserName("root");
    mysql->setPassword("fanjingkai");
    if(!(mysql->open())){
        qDebug()<<"open mysql err";
    }
    /*DeviceData*/
    /*SwitchDevice*/
    display = new QSqlTableModel;
    /*为tableview设置数据模型*/
    ui->DisplaySqlData->setModel(display);
    /*清空数据模型*/
    display->clear();
    /*设置模型的 数据表*/
    display->setTable("SwitchDevice");
    //    /*设置条件显示*/
    //    display->setFilter("username='free_irq@163.com'");
    /*显示数据*/
    display->select();
    /*隐藏列*/
    ui->DisplaySqlData->setColumnHidden(0,true);

    table = TABLE_SWITCH_DEVICE;



    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),SLOT(timeout()));
    timer->start(1000);
}

HistoryData::~HistoryData()
{
    delete ui;
}
#define INSERT_PM25_VALUE "INSERT INTO `DeviceData` (`name`, `value`) VALUES (?, ?);"
void HistoryData::sendPm25Value(uint16_t id, uint16_t value)
{
    QSqlQuery query;
    query.prepare(INSERT_PM25_VALUE);
    query.bindValue(0,QString("PM2.5_%1").arg(id));
    query.bindValue(1,value);
    query.exec();
}
#define SELCT_DEVICE_ID "SELECT id FROM SwitchDevice WHERE id=?;"
#define INSERT_DEVIVE_ID "INSERT INTO `SwitchDevice` (`id`, `name`, `stat`) VALUES (?, ?, ?);"
#define UPDATE_DEIVE_ID "UPDATE `SwitchDevice` SET `stat`=? WHERE `id`=?;"
void HistoryData::sendLedState(uint16_t id, uint8_t power)
{
    QSqlQuery query;
    query.prepare(SELCT_DEVICE_ID);
    query.bindValue(0,id);
    query.exec();
    if(query.first())
    {
        /*在数据库中查询到该id*/
        query.clear();
        query.prepare(UPDATE_DEIVE_ID);
        query.bindValue(0,power);
        query.bindValue(1,id);
        query.exec();

    }else{
        /*在数据库中没有查询到该id*/
        query.clear();
        query.prepare(INSERT_DEVIVE_ID);
        query.bindValue(0,id);
        query.bindValue(1,QString("灯_%1").arg(id));
        query.bindValue(2,power);
        query.exec();
    }

}

void HistoryData::sendMotorValue(uint16_t id, uint16_t value)
{
    QSqlQuery query;
    query.prepare(SELCT_DEVICE_ID);
    query.bindValue(0,id);
    query.exec();
    if(query.first())
    {
        /*在数据库中查询到该id*/
        query.clear();
        query.prepare(UPDATE_DEIVE_ID);
        query.bindValue(0,value);
        query.bindValue(1,id);
        query.exec();

    }else{
        /*在数据库中没有查询到该id*/
        query.clear();
        query.prepare(INSERT_DEVIVE_ID);
        query.bindValue(0,id);
        query.bindValue(1,QString("风扇_%1").arg(id));
        query.bindValue(2,value);
        query.exec();
    }
}

void HistoryData::sendAirQuAlity(uint16_t id, uint16_t value)
{
    QSqlQuery query;
    query.prepare(INSERT_PM25_VALUE);
    query.bindValue(0,QString("综合空气质量_%1").arg(id));
    query.bindValue(1,value);
    query.exec();
}

void HistoryData::sendWaterHeater(uint16_t id, uint16_t value)
{
    QSqlQuery query;
    query.prepare(SELCT_DEVICE_ID);
    query.bindValue(0,id);
    query.exec();
    if(query.first())
    {
        /*在数据库中查询到该id*/
        query.clear();
        query.prepare(UPDATE_DEIVE_ID);
        query.bindValue(0,value);
        query.bindValue(1,id);
        query.exec();

    }else{
        /*在数据库中没有查询到该id*/
        query.clear();
        query.prepare(INSERT_DEVIVE_ID);
        query.bindValue(0,id);
        query.bindValue(1,QString("热水器_%1").arg(id));
        query.bindValue(2,value);
        query.exec();
    }
}

void HistoryData::switch_ui_init()
{
    display->clear();
    /*设置模型的 数据表*/
    switch(table){
    case TABLE_DATE_DEVICE:
        display->setTable("DeviceData");
        display->setSort(0,Qt::DescendingOrder);
        break;
    case TABLE_SWITCH_DEVICE:
        display->setTable("SwitchDevice");
        break;
    }
    display->select();
    /*隐藏列*/
    ui->DisplaySqlData->setColumnHidden(0,true);
}

void HistoryData::on_DataDevice_clicked()
{

    table = TABLE_DATE_DEVICE;
    switch_ui_init();
}

void HistoryData::on_SwitchDevice_clicked()
{
    table = TABLE_SWITCH_DEVICE;
    switch_ui_init();
}

void HistoryData::timeout()
{
    switch_ui_init();
}
