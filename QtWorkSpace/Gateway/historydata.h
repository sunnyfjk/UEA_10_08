#ifndef HISTORYDATA_H
#define HISTORYDATA_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class HistoryData;
}

class HistoryData : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryData(QWidget *parent = 0);
    ~HistoryData();

private:
    Ui::HistoryData *ui;
    QSqlDatabase *mysql;
    QSqlTableModel *display;
};

#endif // HISTORYDATA_H
