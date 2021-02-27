#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //从屏幕上（100，100）位置开始（即为最左上角的点），
    //显示一个200*120的界面（宽200，高120）
    w.setGeometry(100, 100, 500, 300);
    w.show();

    return a.exec();
}
