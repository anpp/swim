#include "swimwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SwiMWindow w;
    w.show();

    return a.exec();
}
