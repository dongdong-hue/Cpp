#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QFile file();
    Qstring str(QDir::currentPath());
}

Widget::~Widget()
{
    delete ui;
}
