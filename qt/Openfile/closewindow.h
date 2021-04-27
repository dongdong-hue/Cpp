#ifndef CLOSEWINDOW_H
#define CLOSEWINDOW_H

#include <QMainWindow>
#include <qstringlist.h>
#include <qdir>
#include <QApplication>

namespace Ui {
class CloseWindow;
}

class CloseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CloseWindow(QWidget *parent = 0);
    ~CloseWindow();
    void InitFilname(QString fileName = QCoreApplication::applicationDirPath());
private slots:
    void on_pushButton_clicked();

    void on_filepath_currentIndexChanged(const QString &arg1);

private:
    Ui::CloseWindow *ui;
    QStringList filelist_;
    QDir dir_;
};

#endif // CLOSEWINDOW_H
