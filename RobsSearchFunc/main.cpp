#include "Rrsearchwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RRSearchWindow w;
    w.setWindowTitle("RsearchFunc - Recipe Search");
    w.resize(1000, 700);
    w.show();
    return a.exec();
}
