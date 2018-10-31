#ifndef TOILET_H
#define TOILET_H

#include <QWidget>
#include <getewaybase.h>

namespace Ui {
class Toilet;
}

class Toilet : public GeteWayBase
{
    Q_OBJECT

public:
    explicit Toilet(QWidget *parent = 0);
    ~Toilet();

private slots:
    void on_dial_valueChanged(int value);

    void on_pushButton_clicked(bool checked);

    void on_pushButton_2_clicked(bool checked);

private:
    Ui::Toilet *ui;
};

#endif // TOILET_H
