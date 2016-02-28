#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_Exit_clicked();

    void on_pushButton_Desktop_clicked();

    void on_pushButton_Command_clicked();

    void on_pushButton_Icon_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_Save_clicked();

private:
    Ui::MainWindow *ui;
    QString str_dir_set;
    QString str_file_set;
    void setList_icon();
    void setIcon_pix(QString &str);
};

#endif // MAINWINDOW_H
