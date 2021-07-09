#include <QApplication>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <qdebug.h>

int add(int a, int b) {
    return a + b;
}
int sub(int a, int b) {
    return a - b;
}
using PTR = int(int, int);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //[2] ����c++11 using �����ͱ�����
    boost::function<PTR> funObj = boost::bind(add, _1, _2);
    qDebug() << funObj(78, 78);
    //[3] ����c++11�����Ƶ���
    auto funObj2 = boost::bind(sub, _1, _2);
    qDebug() << funObj2(87, 78);
    return a.exec();
}
