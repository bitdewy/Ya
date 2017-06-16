/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "mainwindow.h"
#include "opencv2/core.hpp"
#include <iostream>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    using std::cout;
    using std::endl;
    using std::vector;
    using namespace cv;
    Mat I = Mat::eye(4, 4, CV_64F);
    I.at<double>(1,1) = CV_PI;
    cout << "I = \n" << I << ";" << endl << endl;
    Mat r = Mat(10, 3, CV_8UC3);
    randu(r, Scalar::all(0), Scalar::all(255));
    cout << "r (default) = \n" << r << ";" << endl << endl;
    cout << "r (matlab) = \n" << format(r, Formatter::FMT_MATLAB) << ";" << endl << endl;
    cout << "r (python) = \n" << format(r, Formatter::FMT_PYTHON) << ";" << endl << endl;
    cout << "r (numpy) = \n" << format(r, Formatter::FMT_NUMPY) << ";" << endl << endl;
    cout << "r (csv) = \n" << format(r, Formatter::FMT_CSV) << ";" << endl << endl;
    cout << "r (c) = \n" << format(r, Formatter::FMT_C) << ";" << endl << endl;
    Point2f p(5, 1);
    cout << "p = " << p << ";" << endl;
    Point3f p3f(2, 6, 7);
    cout << "p3f = " << p3f << ";" << endl;
    vector<float> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    cout << "shortvec = " << Mat(v) << endl;
    vector<Point2f> points(20);
    for (size_t i = 0; i < points.size(); ++i)
        points[i] = Point2f((float)(i * 5), (float)(i % 7));
    cout << "points = " << points << ";" << endl;
}

MainWindow::~MainWindow()
{

}
