#include "mainwindow.h"
#include <QApplication>


languageType langType;
QTranslator *trans;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
