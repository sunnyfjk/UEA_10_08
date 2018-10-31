#ifndef STUDYROOM_H
#define STUDYROOM_H

#include <QWidget>
#include <getewaybase.h>

namespace Ui {
class StudyRoom;
}

class StudyRoom : public GeteWayBase
{
    Q_OBJECT

public:
    explicit StudyRoom(QWidget *parent = 0);
    ~StudyRoom();

private:
    Ui::StudyRoom *ui;
};

#endif // STUDYROOM_H
