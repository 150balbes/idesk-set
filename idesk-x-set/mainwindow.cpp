#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QPixmap>
#include <QListWidget>
#include <QMessageBox>
#include <signal.h>

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
//    ui->pushButton_Save->hide();
    setList_icon();
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
    if (ui->Idesk_Caption->text() != "")
    {
        int n = QMessageBox::warning(0, "Внимание", "Поле Наименование не пустое. Заполнить параметры новыми данными ? ", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (n == QMessageBox::No)
        {
            return;
        }
    }

    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "/usr/share/applications", "*.desktop");
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

                    setIcon_pix(str_3);
                }
                else if (str_1.startsWith("Name="))
                {
                    ui->Idesk_Caption->setText(str_2);
                    ui->Idesk_CaptionTip->setText(str_2);
                }
            }
//            ui->pushButton_Save->show();
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
    setIcon_pix(str);
}

void MainWindow::on_pushButton_del_clicked()
{
    int n = QMessageBox::warning(0, "Внимание", "Вы действительно хотите удалить выбранную иконку ? ", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (n == QMessageBox::Yes)
    {
//            QMessageBox msgBox;
//            msgBox.setText("Эта функция пока не работает. ");
//            msgBox.move(400, 300);
//            msgBox.exec();
            QString str = ui->List_icon->currentText();
            QFile file(str_dir_set + "/" + str);
            if(file.exists())
            {
                file.remove();
                setList_icon();

                char *prog = "kill -s 10 $(pidof idesk)";
                system(prog);
            }
    }
}

void MainWindow::setList_icon()
{
    QStringList listFile = QDir(str_dir_set).entryList(QDir::Files);
    foreach (QString file, listFile) {
        ui->List_icon->addItem(file);
    }
}

void MainWindow::on_pushButton_edit_clicked()
{
    if (ui->Idesk_Caption->text() != "")
    {
        int n = QMessageBox::warning(0, "Внимание", "Поле Наименование не пустое. Заполнить параметры новыми данными ? ", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (n == QMessageBox::No)
        {
            return;
        }
    }
    QString str = ui->List_icon->currentText();
    QFile file(str_dir_set + "/" + str);
    if(file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
       while(!file.atEnd())
       {
           QString str_1 = file.readLine();
           str_1 = str_1.trimmed();
           if (str_1.startsWith("Caption:")) {
               str_1 = str_1.mid(9);
               ui->Idesk_Caption->setText(str_1);
           } else if (str_1.startsWith("CaptionTip:")) {
               str_1 = str_1.mid(12);
               ui->Idesk_CaptionTip->setText(str_1);
           } else if (str_1.startsWith("Command:")) {
               str_1 = str_1.mid(9);
               ui->Idesk_Command->setText(str_1);
           } else if (str_1.startsWith("Icon:")) {
               str_1 = str_1.mid(6);
               ui->Idesk_Icon->setText(str_1);
           } else if (str_1.startsWith("Width:")) {
               str_1 = str_1.mid(7);
               ui->Idesk_Width->setValue(str_1.toInt());
           } else if (str_1.startsWith("Height:")) {
               str_1 = str_1.mid(8);
               ui->Idesk_Height->setValue(str_1.toInt());
           } else if (str_1.startsWith("X:")) {
               str_1 = str_1.mid(3);
               ui->Idesk_X->setValue(str_1.toInt());
           } else if (str_1.startsWith("Y:")) {
               str_1 = str_1.mid(3);
               ui->Idesk_Y->setValue(str_1.toInt());
           }
        }
       QString str = ui->Idesk_Icon->text();
       setIcon_pix(str);

    }
}

void MainWindow::setIcon_pix(QString & str)
{
    QPixmap myPixmap(str);
    ui->label_prev->setScaledContents(1);
    ui->label_prev->setPixmap(myPixmap);
}

void MainWindow::on_pushButton_Save_clicked()
{
            if (ui->Idesk_Caption->text() == "")
            {
                QMessageBox msgBox;
                msgBox.setText("Не заполнено поле Название.");
                msgBox.move(400, 300);
                msgBox.exec();
                return;
            }

            if (ui->Idesk_Caption->text() != "")
            {
                int n = QMessageBox::warning(0, "Внимание", "Сохранить данные в файле и перезапустить Idesk ? ", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                if (n == QMessageBox::No)
                {
                    return;
                }
            }

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

            char *prog = "kill -s 10 $(pidof idesk)";
            system(prog);

            setList_icon();

}
