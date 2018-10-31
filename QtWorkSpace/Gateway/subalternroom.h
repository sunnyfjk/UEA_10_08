#ifndef SUBALTERNROOM_H
#define SUBALTERNROOM_H

#include <QWidget>
#include <getewaybase.h>

namespace Ui {
class SubalternRoom;
}

class SubalternRoom : public GeteWayBase
{
    Q_OBJECT

public:
    explicit SubalternRoom(QWidget *parent = 0);
    ~SubalternRoom();

private:
    Ui::SubalternRoom *ui;
};

#endif // SUBALTERNROOM_H
