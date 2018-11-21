#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <iostream>
int parametersetA, parametersetB;
int main(int argc, char *argv[])
{
	std::cin >> parametersetA;
	std::cin >> parametersetA;
	QApplication a(argc, argv);
    MainWindow w;
	
    for (int i = 1; i < argc; ++i)
    {
        QString current(argv[i]);
        QFile file(current);
        if (file.exists())
            w.loadFile(current);
    }

    w.show();
	
    return a.exec();
}
