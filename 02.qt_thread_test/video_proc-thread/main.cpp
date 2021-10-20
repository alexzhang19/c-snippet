#include <QCoreApplication>
#include <QMetaType>
#include <iostream>
#include <QThread>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "qmanager.h"

using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qRegisterMetaType<cv::Mat>("cv::Mat&");

    qDebug()<< "main(), tid :" << QThread::currentThreadId();
    QManager app("/home/data/videos/TownCentreXVID.avi");
    app.run();

    return a.exec();
}
