#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
#include <QPushButton>
#include <QString>

static const double PI = 3.14;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    bool ok;
    QString tempstr;
    QString valueStr = ui->radiusLineEdit->text();
    int valueInt = valueStr.toInt(&ok);
    double area = valueInt * valueInt * PI;
    ui->areaLabel_2->setText(tempstr.setNum(area));
}
