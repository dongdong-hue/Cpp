#include "openfilewindow.h"
#include "ui_openfilewindow.h"
#include "closewindow.h"
#include <iostream>
OpenFileWindow::OpenFileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenFileWindow)
{
    ui->setupUi(this);
    connect(close_, );
}

OpenFileWindow::~OpenFileWindow()
{
    std::cout << "~OpenFileWindow()" << std::endl;
    delete close_;
    delete ui;
}

void OpenFileWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "save")
        std::cout << arg1.toStdString() << std::endl;
    else if (arg1 == "open")
    {
        close_ = new CloseWindow;
        close_->show();
    }
    else if (arg1 == "close")
    {
//        close_ = new CloseWindow;
//        close_->show();
    }
}

void OpenFileWindow::on_start_clicked()
{
    GetFilename();
}

void GetFilename()
{

}

