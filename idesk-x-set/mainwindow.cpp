#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    str_dir_set = (QDir::homePath()+"/.idesktop");

    if (!QDir(str_dir_set).exists())
    {
        QDir dir;
        dir.mkdir(str_dir_set);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Exit_clicked()
{
    close();
}

void MainWindow::on_pushButton_Desktop_clicked()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "/usr/share/applications", "*.desktop");
    ui->Desktop->setText(str);


    QFile file(str);
        if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!file.atEnd())
            {
                QString str_1 = file.readLine();
                QString str_2 = str_1.mid(5);
                str_2 = str_2.toLower();
                str_2 = str_2.trimmed();
                if (str_1.startsWith("Exec="))
                {
                    ui->Idesk_Command->setText(str_2);
                }
                else if (str_1.startsWith("Icon="))
                {
                    QString str_3 = "/usr/share/icons/hicolor/48x48/apps/" + str_2 + ".png";
                    ui->Idesk_Icon->setText(str_3);
                }
                else if (str_1.startsWith("Name="))
                {
                    ui->Idesk_Caption->setText(str_2);
                    ui->Idesk_CaptionTip->setText(str_2);
                }
            }
        }

}

void MainWindow::on_pushButton_Save_clicked()
{
    if (ui->Idesk_Caption->text() != "")
        {
            str_file_set = str_dir_set + "/" + ui->Idesk_Caption->text() + ".lnk";
            QFile fileSet(str_file_set);
            fileSet.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream writeStream(&fileSet);

            QString str_check;
            str_check = "table Icon\n";
            writeStream << str_check;
            str_check = "  Caption: " + ui->Idesk_Caption->text() + "\n";
            writeStream << str_check;
            str_check = "  CaptionTip: " + ui->Idesk_CaptionTip->text() + "\n";
            writeStream << str_check;
            str_check = "  Command: " + ui->Idesk_Command->text() + "\n";
            writeStream << str_check;
            str_check = "  Icon: " + ui->Idesk_Icon->text() + "\n";
            writeStream << str_check;
            str_check = "  Width: " + ui->Idesk_Width->text() + "\n";
            writeStream << str_check;
            str_check = "  Height: " + ui->Idesk_Height->text() + "\n";
            writeStream << str_check;
            str_check = "  X: " + ui->Idesk_X->text() + "\n";
            writeStream << str_check;
            str_check = "  Y: " + ui->Idesk_Y->text() + "\n";
            writeStream << str_check;
            str_check = "end\n";
            writeStream << str_check;
            fileSet.close();

            m_process = new QProcess(this);
            QString str_prog = "kill -s 16 $(pgrep idesk)";
            m_process->start(str_prog);
        }
}

void MainWindow::on_pushButton_Command_clicked()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "/usr/bin/" + ui->Idesk_Command->text());
    ui->Idesk_Command->setText(str);
}

void MainWindow::on_pushButton_Icon_clicked()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", ui->Idesk_Icon->text(),"*.png *.jpg *.jpeg");
    ui->Idesk_Icon->setText(str);
}
