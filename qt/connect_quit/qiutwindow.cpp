#include "qiutwindow.h"
#include "ui_qiutwindow.h"

QiutWindow::QiutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QiutWindow)
{
    ui->setupUi(this);
}

QiutWindow::~QiutWindow()
{
    delete ui;
}
