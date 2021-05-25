#include "qiutwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QiutWindow w;
    w.show();

    return a.exec();
}
