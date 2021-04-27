#ifndef QUITWINDOW_H
#define QUITWINDOW_H

#include <QMainWindow>

class quitWindow : public QMainWindow
{
    Q_OBJECT

public:
    quitWindow(QWidget *parent = 0);
    ~quitWindow();
};

#endif // QUITWINDOW_H
