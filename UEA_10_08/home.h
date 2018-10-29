#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class home;
}

class home : public QWidget
{
    Q_OBJECT

public:
    explicit home(QWidget *parent = 0);
    ~home();
#pragma  pack(push,1)
    struct Stm32State_t
    {
        uint8_t state;
        char pass[10];
    };
#pragma pack(pop)
protected slots:
    void error(QSerialPort::SerialPortError serialPortError);
    void readyRead();
private slots:
    void on_pushButton_clicked();

private:
    Ui::home *ui;
    QSerialPort *SerialPort;
};

#endif // HOME_H
