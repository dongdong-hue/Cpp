#include "widget.h"
#include "ui_widget.h"
#include <qstring>
#include <qdir>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QFile file();
    QString str(QDir::currentPath());

}

Widget::~Widget()
{
    delete ui;
}
