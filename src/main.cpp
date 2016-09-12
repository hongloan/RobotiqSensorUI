/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w=new MainWindow;
    w->setFixedSize(670,470);
    w->setWindowTitle("Robotiq Sensor User Interface");
    w->show();
    return a.exec();
}
