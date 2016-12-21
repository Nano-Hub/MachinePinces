#include <iostream>
#include "mainwindow.h"
#include "opencv2/opencv.hpp"
#include <QApplication>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Projet 3D - (Blanc - Alder)");
    w.show();
    return a.exec();
    return 0;
}
