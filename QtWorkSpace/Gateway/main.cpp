#include "home.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    home w;
    w.show();
    /*
    BedRoom 卧室
    CookRoom 厨房
    Parlour 客厅
    StudyRoom 书房
    SubalternRoom 次卧
    Toilet 卫生间
    */
    return a.exec();
}
