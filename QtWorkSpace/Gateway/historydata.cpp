#include "historydata.h"
#include "ui_historydata.h"
#include <QDebug>

HistoryData::HistoryData(QWidget *parent) :
    QWidget(parent),
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



    display = new QSqlTableModel;
    /*为tableview设置数据模型*/
    ui->DisplaySqlData->setModel(display);
    /*清空数据模型*/
    display->clear();
    /*设置模型的 数据表*/
    display->setTable("mqtt_user");
    /*设置条件显示*/
    display->setFilter("username='free_irq@163.com'");
    /*显示数据*/
    display->select();
    /*隐藏列*/
    ui->DisplaySqlData->setColumnHidden(0,true);

}

HistoryData::~HistoryData()
{
    delete ui;
}
