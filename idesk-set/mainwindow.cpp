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
    setIdesk_def();
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
    ui->List_icon->clear();
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
            Icon_clear();

}

void MainWindow::Icon_clear()
{
 ui->Idesk_Caption->clear();
 ui->Idesk_CaptionTip->clear();
 ui->Idesk_Command->clear();
 ui->Idesk_Icon->clear();
 ui->label_prev->clear();
}

void MainWindow::setIdesk_def()
{
    QString str_file_def = (QDir::homePath() + "/.ideskrc");
    QFile fileDef(str_file_def);
    if (!fileDef.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("Не найден файл настроек.");
        msgBox.move(400, 300);
        msgBox.exec();
        return;
    }
    fileDef.open(QIODevice::ReadWrite | QIODevice::Text);
    while(!fileDef.atEnd())
    {
        QString str = fileDef.readLine();
        str = str.trimmed();
        if (str.startsWith("FontName:")) {
            str = str.mid(10);
            ui->IdeskSet_FontName->setText(str);
        } else if (str.startsWith("FontSize:")) {
            str = str.mid(9);
            ui->IdeskSet_FontSize->setValue(str.toInt());
        } else if (str.startsWith("FontColor:")) {
            str = str.mid(11);
            ui->IdeskSet_FontColor->setText(str);
        } else if (str.startsWith("FontNameTip:")) {
            str = str.mid(13);
            ui->IdeskSet_FontNameTip->setText(str);
        } else if (str.startsWith("FontSizeTip:")) {
            str = str.mid(12);
            ui->IdeskSet_FontSizeTip->setValue(str.toInt());
        } else if (str.startsWith("ForeColorTip:")) {
            str = str.mid(14);
            ui->IdeskSet_ForeColorTip->setText(str);
        } else if (str.startsWith("BackColorTip:")) {
            str = str.mid(14);
            ui->IdeskSet_BackColorTip->setText(str);
        } else if (str.startsWith("PaddingX:")) {
            str = str.mid(9);
            ui->IdeskSet_PaddingX->setValue(str.toInt());
        } else if (str.startsWith("PaddingY:")) {
            str = str.mid(9);
            ui->IdeskSet_PaddingY->setValue(str.toInt());
        } else if (str.startsWith("Transparency:")) {
            str = str.mid(13);
            ui->IdeskSet_Transparency->setValue(str.toInt());
        } else if (str.startsWith("HighContrast:")) {
            str = str.mid(14);
            ui->IdeskSet_HighContrast->setText(str);
        } else if (str.startsWith("Locked:")) {
            str = str.mid(8);
            ui->IdeskSet_Locked->setText(str);
        } else if (str.startsWith("Shadow:")) {
            str = str.mid(8);
            ui->IdeskSet_Shadow->setText(str);
        } else if (str.startsWith("ShadowColor:")) {
            str = str.mid(13);
            ui->IdeskSet_ShadowColor->setText(str);
        } else if (str.startsWith("ShadowX:")) {
            str = str.mid(9);
            ui->IdeskSet_ShadowX->setValue(str.toInt());
        } else if (str.startsWith("ShadowY:")) {
            str = str.mid(9);
            ui->IdeskSet_ShadowY->setValue(str.toInt());
        } else if (str.startsWith("SnapShadow:")) {
            str = str.mid(12);
            ui->IdeskSet_SnapShadow->setText(str);
        } else if (str.startsWith("IconSnap:")) {
            str = str.mid(10);
            ui->IdeskSet_IconSnap->setText(str);
        } else if (str.startsWith("SnapWidth:")) {
            str = str.mid(11);
            ui->IdeskSet_SnapWidth->setValue(str.toInt());
        } else if (str.startsWith("SnapHeight:")) {
            str = str.mid(12);
            ui->IdeskSet_SnapHeight->setValue(str.toInt());
        } else if (str.startsWith("SnapOrigin:")) {
            str = str.mid(12);
            ui->IdeskSet_SnapOrigin->setText(str);
        } else if (str.startsWith("CaptionOnHover:")) {
            str = str.mid(16);
            ui->IdeskSet_CaptionOnHover->setText(str);
        } else if (str.startsWith("CaptionTipOnHover:")) {
            str = str.mid(19);
            ui->IdeskSet_CaptionTipOnHover->setText(str);
        } else if (str.startsWith("CaptionTipPlacement:")) {
            str = str.mid(21);
            ui->IdeskSet_CaptionTipPlacement->setText(str);
        } else if (str.startsWith("CaptionPlacement:")) {
            str = str.mid(18);
            ui->IdeskSet_CaptionPlacement->setText(str);
        } else if (str.startsWith("Bold:")) {
            str = str.mid(6);
            ui->IdeskSet_Bold->setText(str);
        } else if (str.startsWith("ToolTip.FontSize:")) {
            str = str.mid(18);
            ui->IdeskSet_ToolTip_FontSize->setValue(str.toInt());
        } else if (str.startsWith("ToolTip.FontName:")) {
            str = str.mid(18);
            ui->IdeskSet_ToolTip_FontName->setText(str);
        } else if (str.startsWith("ToolTip.ForeColor:")) {
            str = str.mid(19);
            ui->IdeskSet_ToolTip_ForeColor->setText(str);
        } else if (str.startsWith("ToolTip.BackColor:")) {
            str = str.mid(19);
            ui->IdeskSet_ToolTip_BackColor->setText(str);
        } else if (str.startsWith("ToolTip.CaptionOnHover:")) {
            str = str.mid(24);
            ui->IdeskSet_ToolTip_CaptionOnHover->setText(str);
        } else if (str.startsWith("ToolTip.CaptionPlacement:")) {
            str = str.mid(26);
            ui->IdeskSet_ToolTip_CaptionPlacement->setText(str);
        } else if (str.startsWith("Background.Delay:")) {
            str = str.mid(18);
            ui->IdeskSet_Background_Delay->setValue(str.toInt());
        } else if (str.startsWith("Background.Source:")) {
            str = str.mid(19);
            ui->IdeskSet_Background_Source->setText(str);
        } else if (str.startsWith("Background.File:")) {
            str = str.mid(17);
            ui->IdeskSet_Background_File->setText(str);
        } else if (str.startsWith("Background.Mode:")) {
            str = str.mid(17);
            ui->IdeskSet_Background_Mode->setText(str);
        } else if (str.startsWith("Background.Color:")) {
            str = str.mid(18);
            ui->IdeskSet_Background_Color->setText(str);
        } else if (str.startsWith("Lock:")) {
            str = str.mid(6);
            ui->IdeskSet_Lock->setText(str);
        } else if (str.startsWith("Reload:")) {
            str = str.mid(8);
            ui->IdeskSet_Reload->setText(str);
        } else if (str.startsWith("Drag:")) {
            str = str.mid(6);
            ui->IdeskSet_Drag->setText(str);
        } else if (str.startsWith("EndDrag:")) {
            str = str.mid(9);
            ui->IdeskSet_EndDrag->setText(str);
        } else if (str.startsWith("Execute[0]:")) {
            str = str.mid(12);
            ui->IdeskSet_Execute0->setText(str);
        } else if (str.startsWith("Execute[1]:")) {
            str = str.mid(12);
            ui->IdeskSet_Execute1->setText(str);
        }

     }

}

void MainWindow::on_pushButton_clicked()
{
    int n = QMessageBox::warning(0, "Внимание", "Сохранить общие настройки в файле \n .ideskrc  \n и перезапустить Idesk ? ", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (n == QMessageBox::No)
    {
        return;
    }

    QString str_file_def = (QDir::homePath() + "/.ideskrc");
    QFile fileDef(str_file_def);
    if (!fileDef.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("Не найден файл настроек .ideskrc ! \n Настройки не сохранены !");
        msgBox.move(400, 300);
        msgBox.exec();
        return;
    }
    fileDef.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream writeStream(&fileDef);

    QString str_check;
    str_check = "table Config\n";
    writeStream << str_check;
    str_check = "  FontName: " + ui->IdeskSet_FontName->text() + "\n";
    writeStream << str_check;
    str_check = "  FontSize: " + ui->IdeskSet_FontSize->text() + "\n";
    writeStream << str_check;
    str_check = "  FontColor: " + ui->IdeskSet_FontColor->text() + "\n";
    writeStream << str_check;
    str_check = "  FontNameTip: " + ui->IdeskSet_FontNameTip->text() + "\n";
    writeStream << str_check;
    str_check = "  FontSizeTip: " + ui->IdeskSet_FontSizeTip->text() + "\n";
    writeStream << str_check;
    str_check = "  ForeColorTip: " + ui->IdeskSet_ForeColorTip->text() + "\n";
    writeStream << str_check;
    str_check = "  BackColorTip: " + ui->IdeskSet_BackColorTip->text() + "\n";
    writeStream << str_check;
    str_check = "  PaddingX: " + ui->IdeskSet_PaddingX->text() + "\n";
    writeStream << str_check;
    str_check = "  PaddingY: " + ui->IdeskSet_PaddingY->text() + "\n";
    writeStream << str_check;
    str_check = "  Transparency: " + ui->IdeskSet_Transparency->text() + "\n";
    writeStream << str_check;
    str_check = "  HighContrast: " + ui->IdeskSet_HighContrast->text() + "\n";
    writeStream << str_check;
    str_check = "  Locked: " + ui->IdeskSet_Locked->text() + "\n";
    writeStream << str_check;
    str_check = "  Shadow: " + ui->IdeskSet_Shadow->text() + "\n";
    writeStream << str_check;
    str_check = "  ShadowColor: " + ui->IdeskSet_ShadowColor->text() + "\n";
    writeStream << str_check;
    str_check = "  ShadowX: " + ui->IdeskSet_ShadowX->text() + "\n";
    writeStream << str_check;
    str_check = "  ShadowY: " + ui->IdeskSet_ShadowY->text() + "\n";
    writeStream << str_check;
    str_check = "  SnapShadow: " + ui->IdeskSet_SnapShadow->text() + "\n";
    writeStream << str_check;
    str_check = "  IconSnap: " + ui->IdeskSet_IconSnap->text() + "\n";
    writeStream << str_check;
    str_check = "  SnapWidth: " + ui->IdeskSet_SnapWidth->text() + "\n";
    writeStream << str_check;
    str_check = "  SnapHeight: " + ui->IdeskSet_SnapHeight->text() + "\n";
    writeStream << str_check;

    str_check = "  SnapOrigin: " + ui->IdeskSet_SnapOrigin->text() + "\n";
    writeStream << str_check;
    str_check = "  CaptionOnHover: " + ui->IdeskSet_CaptionOnHover->text() + "\n";
    writeStream << str_check;
    str_check = "  CaptionTipOnHover: " + ui->IdeskSet_CaptionTipOnHover->text() + "\n";
    writeStream << str_check;
    str_check = "  CaptionTipPlacement: " + ui->IdeskSet_CaptionTipPlacement->text() + "\n";
    writeStream << str_check;
    str_check = "  CaptionPlacement: " + ui->IdeskSet_CaptionPlacement->text() + "\n";
    writeStream << str_check;
    str_check = "  Bold: " + ui->IdeskSet_Bold->text() + "\n";
    writeStream << str_check;
    str_check = "  ToolTip.FontSize: " + ui->IdeskSet_ToolTip_FontSize->text() + "\n";
    writeStream << str_check;
    str_check = "  ToolTip.FontName: " + ui->IdeskSet_ToolTip_FontName->text() + "\n";
    writeStream << str_check;
    str_check = "  ToolTip.ForeColor: " + ui->IdeskSet_ToolTip_ForeColor->text() + "\n";
    writeStream << str_check;
    str_check = "  ToolTip.BackColor: " + ui->IdeskSet_ToolTip_BackColor->text() + "\n";
    writeStream << str_check;
    str_check = "  ToolTip.CaptionOnHover: " + ui->IdeskSet_ToolTip_CaptionOnHover->text() + "\n";
    writeStream << str_check;
    str_check = "  ToolTip.CaptionPlacement: " + ui->IdeskSet_ToolTip_CaptionPlacement->text() + "\n";
    writeStream << str_check;
    str_check = "  Background.Delay: " + ui->IdeskSet_Background_Delay->text() + "\n";
    writeStream << str_check;
    str_check = "  Background.Source: " + ui->IdeskSet_Background_Source->text() + "\n";
    writeStream << str_check;
    str_check = "  Background.File: " + ui->IdeskSet_Background_File->text() + "\n";
    writeStream << str_check;
    str_check = "  Background.Mode: " + ui->IdeskSet_Background_Mode->text() + "\n";
    writeStream << str_check;
    str_check = "  Background.Color: " + ui->IdeskSet_Background_Color->text() + "\n";
    writeStream << str_check;
    str_check = "end\n";
    writeStream << str_check;

    str_check = "\n";
    writeStream << str_check;

    str_check = "table Actions\n";
    writeStream << str_check;

    str_check = "  Lock: " + ui->IdeskSet_Lock->text() + "\n";
    writeStream << str_check;
    str_check = "  Reload: " + ui->IdeskSet_Reload->text() + "\n";
    writeStream << str_check;
    str_check = "  Drag: " + ui->IdeskSet_Drag->text() + "\n";
    writeStream << str_check;
    str_check = "  EndDrag: " + ui->IdeskSet_EndDrag->text() + "\n";
    writeStream << str_check;
    str_check = "  Execute[0]: " + ui->IdeskSet_Execute0->text() + "\n";
    writeStream << str_check;
    str_check = "  Execute[1]: " + ui->IdeskSet_Execute1->text() + "\n";
    writeStream << str_check;

    str_check = "end\n";
    writeStream << str_check;
    fileDef.close();

    char *prog = "kill -s 10 $(pidof idesk)";
    system(prog);

}
