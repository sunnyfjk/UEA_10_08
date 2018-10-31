#ifndef PARLOUR_H
#define PARLOUR_H

#include <QWidget>
#include <getewaybase.h>

namespace Ui {
class Parlour;
}

class Parlour : public GeteWayBase
{
    Q_OBJECT

public:
    explicit Parlour(QWidget *parent = 0);
    ~Parlour();

private:
    Ui::Parlour *ui;
};

#endif // PARLOUR_H
