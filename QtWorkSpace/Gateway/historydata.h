#ifndef HISTORYDATA_H
#define HISTORYDATA_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <getewaybase.h>
#include <QTimer>

namespace Ui {
class HistoryData;
}

class HistoryData : public GeteWayBase
{
    Q_OBJECT

public:
    explicit HistoryData(QWidget *parent = 0);
    ~HistoryData();
    enum SqlTable_t{
        TABLE_DATE_DEVICE,
        TABLE_SWITCH_DEVICE,
    };
public slots:
    void sendPm25Value(uint16_t id,uint16_t value);
    void sendLedState(uint16_t id,uint8_t power);
    void sendMotorValue(uint16_t id,uint16_t value);
    void sendAirQuAlity(uint16_t id,uint16_t value);
    void sendWaterHeater(uint16_t id,uint16_t value);
    void switch_ui_init();

private slots:
    void on_DataDevice_clicked();
    void on_SwitchDevice_clicked();
    void timeout();
    void createDataBases();
private:
    Ui::HistoryData *ui;
    QSqlDatabase *mysql;
    QSqlTableModel *display;
    QTimer *timer;
    enum SqlTable_t table;

};

#endif // HISTORYDATA_H
