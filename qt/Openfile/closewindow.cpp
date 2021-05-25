#include "closewindow.h"
#include "ui_closewindow.h"
#include <qstring>
#include <qdir>
#include <qdebug.h>
CloseWindow::CloseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CloseWindow)
{
    ui->setupUi(this);
    InitFilname();
}

CloseWindow::~CloseWindow()
{
    delete ui;
}

void CloseWindow::InitFilname(QString fileName)
{
    //获取程序当前运行目录
    //QString fileName = QCoreApplication::applicationDirPath();
//    QDir dir;
    dir_.cd(fileName);
    filelist_ = dir_.entryList();
    ui->filepath->addItems(filelist_);
    for(int i=0; i<filelist_.count(); i++)
    {
        qDebug() << filelist_[i];
    }
}

void CloseWindow::on_pushButton_clicked()
{
    //QObject::connect(&ui->pushButton, &QPushButton::clicked, &QMainWindow::close);
    //ui->pushButton->close();
    this->close();
}

void CloseWindow::on_filepath_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "..")
    {
        dir_.cdUp();
        filelist_.clear();
        ui->filepath->clear();
        filelist_ = dir_.entryList();
        ui->filepath->addItems(filelist_);
        for(int i=0; i<filelist_.count(); i++)
        {
            qDebug() << filelist_[i];
        }
    }
    else if(arg1 == ".")
    {

    }
    else
    {
        dir_.dirName():
        dir_.cd(arg1);
        filelist_.clear();
        ui->filepath->clear();
        filelist_ = dir_.entryList();
        ui->filepath->addItems(filelist_);
        for(int i=0; i<filelist_.count(); i++)
        {
            qDebug() << filelist_[i];
        }
    }
}
