#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <getewaybase.h>
#include <bedroom.h>
#include <cookroom.h>
#include <parlour.h>
#include <studyroom.h>
#include <subalternroom.h>
#include <toilet.h>
#include <stm32f407.h>
#include <historydata.h>
namespace Ui {
class home;
}

class home : public GeteWayBase
{
    Q_OBJECT

public:
    explicit home(QWidget *parent = 0);
    ~home();
public slots:
    void switch_ui(GeteWayBase::GeteWayBaseUi_t id);

private slots:
    void on_Parlour_clicked();

    void on_StudyRoom_clicked();

    void on_BedRoom_clicked();

    void on_SubalternRoom_clicked();

    void on_CookRoom_clicked();

    void on_Toilet_clicked();

    void on_HistoryData_clicked();

private:
    Ui::home *ui;
    BedRoom *bedroom;
    CookRoom *cookroom;
    Parlour *parlour;
    StudyRoom *studyroom;
    SubalternRoom *subalternroom;
    Toilet *toilet;
    HistoryData *histoydata;
    STM32F407 *stm32;
};

#endif // HOME_H
