#ifndef BEDROOM_H
#define BEDROOM_H

#include <QWidget>
#include <getewaybase.h>

namespace Ui {
class BedRoom;
}

class BedRoom : public GeteWayBase
{
    Q_OBJECT

public:
    explicit BedRoom(QWidget *parent = 0);
    ~BedRoom();

private:
    Ui::BedRoom *ui;
};

#endif // BEDROOM_H
