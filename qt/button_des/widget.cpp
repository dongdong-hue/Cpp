#include "widget.h"
//#include "ui_widget.h"
#include <qapplication.h>
#include <qpushbutton.h>
#include <qfont.h>
#include <qlabel.h>
#include <qdatetime.h>
#include <synchapi.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::Widget)
{
    //Initquit_button();

    datetime_get();
    //ui->setupUi(this);
}

Widget::~Widget()
{
    //delete datalabel;
    //delete datatime;
    //delete ui;
}

void Widget::Initquit_button()
{
    setMinimumSize(200, 120);
    setMaximumSize(200, 120);
    QPushButton* quit = new QPushButton( "Qiut", this);
    quit->setGeometry(62, 40, 75, 30);
    quit->setFont(QFont("Times", 18, QFont::ExtraLight ));
    connect( quit, SIGNAL(clicked(bool)), qApp, SLOT(quit()));
}

void Widget::datetime_get()
{
    setMinimumSize(500, 220);
    setMaximumSize(500, 220);
    QLabel* datalabel = new QLabel();
    QDateTime* datatime = new QDateTime(QDateTime::\
                                  currentDateTime());
    //while(1)
    //{
        *datatime = QDateTime::currentDateTime();
        datalabel->setText(datatime->date().toString());
        datalabel->show();
        //Sleep(500);
    //}
}
