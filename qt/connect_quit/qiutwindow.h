#ifndef QIUTWINDOW_H
#define QIUTWINDOW_H

#include <QMainWindow>

namespace Ui {
class QiutWindow;
}

class QiutWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QiutWindow(QWidget *parent = 0);
    ~QiutWindow();

private:
    Ui::QiutWindow *ui;
};

#endif // QIUTWINDOW_H
