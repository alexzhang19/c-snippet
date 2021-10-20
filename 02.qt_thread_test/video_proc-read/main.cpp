#include <QCoreApplication>
#include <QMetaType>
#include <QDebug>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qRegisterMetaType<cv::Mat>("cv::Mat&");

    cv::VideoCapture cap;
    cap.open("/home/data/videos/TownCentreXVID.avi");
    if(!cap.isOpened())
    {
        qDebug() << " ===> please check the video path.";
        return 0;
    }

    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::Mat frame;
    int nFrame = 0;

    cv::namedWindow("video", 0);
    cv::resizeWindow("video", 960, 480);
    cv::moveWindow("video", 200, 100);

    while(true)
    {
        int frame_id = cap.get(cv::CAP_PROP_POS_FRAMES);
        std::cout << " frame_id " << frame_id << std::endl;

        double t = (double)cv::getTickCount();
        cap.read(frame);
        if (frame.empty())
        {
            qDebug() << " ===> image empty.";
            continue;
        }
        nFrame += 1;

        t = ((double)cv::getTickCount() - t) * 1000 / cv::getTickFrequency();
        cout << " nFrame = " << nFrame << ", Decode time for a frame : " << t << "ms" << endl;

        cv::imshow("video", frame);
        cv::waitKey(10);
    }
    cap.release();

    return a.exec();
}
