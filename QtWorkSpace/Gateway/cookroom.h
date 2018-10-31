#ifndef COOKROOM_H
#define COOKROOM_H

#include <QWidget>
#include <getewaybase.h>

namespace Ui {
class CookRoom;
}

class CookRoom : public GeteWayBase
{
    Q_OBJECT

public:
    explicit CookRoom(QWidget *parent = 0);
    ~CookRoom();

private:
    Ui::CookRoom *ui;
};

#endif // COOKROOM_H
