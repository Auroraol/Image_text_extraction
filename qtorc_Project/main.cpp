#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QMetaType>

int main(int argc, char* argv[])
{
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qRegisterMetaType<std::vector<std::string>>("std::vector<std::string>");
    return a.exec();
}

