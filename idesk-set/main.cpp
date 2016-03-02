#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);

    QApplication a(argc, argv);

    QString langName = QLocale::system().name();
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + langName, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
    QTranslator qtBaseTranslator;
    qtBaseTranslator.load("qtbase_" + langName, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtBaseTranslator);

    MainWindow w;
    w.resize(620, 480);
    w.move(0, 0);
    w.show();

    return a.exec();
}
