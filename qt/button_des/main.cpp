#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //����Ļ�ϣ�100��100��λ�ÿ�ʼ����Ϊ�����Ͻǵĵ㣩��
    //��ʾһ��200*120�Ľ��棨��200����120��
    w.setGeometry(100, 100, 500, 300);
    w.show();

    return a.exec();
}
