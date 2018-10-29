#include "home.h"
#include "ui_home.h"
#include <QMessageBox>
#include <QDebug>

home::home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::home)
{
    ui->setupUi(this);
    SerialPort = new QSerialPort;
    connect(SerialPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(error(QSerialPort::SerialPortError)));
    connect(SerialPort,SIGNAL(readyRead()),this,SLOT(readyRead()));
    SerialPort->setPortName("/dev/ttyUSB0");
    SerialPort->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
    SerialPort->setDataBits(QSerialPort::Data8);
    SerialPort->setParity(QSerialPort::NoParity);
    SerialPort->setStopBits(QSerialPort::OneStop);
    SerialPort->setFlowControl(QSerialPort::NoFlowControl);
    if(!SerialPort->open(QIODevice::ReadWrite)){
        QMessageBox::about(this,"Serial Port Open Err\n","/dev/ttyUSB0 Open Err");
    }
}

home::~home()
{
    delete ui;
}

void home::error(QSerialPort::SerialPortError serialPortError)
{
    if(serialPortError!=QSerialPort::NoError)
        QMessageBox::about(this,"SerialPortError",QString("Serial Port Error num :%1").arg(serialPortError));
}

void home::readyRead()
{
    struct Stm32State_t state;
    int size=SerialPort->read((char *)(&state),sizeof(state));
    qDebug()<<"size="<<size<<"|sizeof(state):"<<sizeof(state);
    qDebug()<<"state.state:"<<state.state<<"|state.pass:"<<QString(state.pass);

}

void home::on_pushButton_clicked()
{
    struct Stm32State_t state={
        1,"123456",
    };
    SerialPort->write((char *)(&state),sizeof(state));

}
