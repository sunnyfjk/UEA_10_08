#include "home.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int nIndex = QFontDatabase::addApplicationFont(
                "/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc");
    if(nIndex != -1)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)
        {
            QFont fontThis(strList.at(0));
            fontThis.setPointSize(7);
            a.setFont(fontThis);
        }
    }

    home w;
    w.show();
    return a.exec();
}
