#ifndef OPENFILEWINDOW_H
#define OPENFILEWINDOW_H

#include <QMainWindow>
#include "closewindow.h"
namespace Ui {
class OpenFileWindow;
}

class OpenFileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenFileWindow(QWidget *parent = 0);
    ~OpenFileWindow();

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_start_clicked();

private:
    Ui::OpenFileWindow *ui;
    CloseWindow* close_;
};

#endif // OPENFILEWINDOW_H
