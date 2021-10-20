#include "qmanager.h"
#include <QDebug>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include "g_base.h"

using namespace cv;


QManager::QManager(const std::string& url):m_reader(url)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    connect(&m_proc, SIGNAL(processed(cv::Mat &, int)), this, SLOT(onImageProcessed(cv::Mat &, int)));

    m_t1 = new QThread();
    m_reader.moveToThread(m_t1);
    QObject::connect(m_t1, &QThread::started, &m_reader, &QReader::run);

    m_t2 = new QThread();
    m_proc.moveToThread(m_t2);
    QObject::connect(m_t2, &QThread::started, &m_proc, &QProcess::run);

//    m_timer.start(40); // 定时器40ms读取一次数据
    m_t1->start();
    m_t2->start();
}


int QManager::run()
{
    cv::namedWindow("video", 0);
    cv::resizeWindow("video", 960, 480);
    cv::moveWindow("video", 200, 100);
    return 0;
}


void QManager::onImageProcessed(cv::Mat &img, int ret)
{
    double t = (double)cv::getTickCount();
    qDebug()<< " QManager onImageProcessed(), tid :" << QThread::currentThreadId();
    int nFrame = ret;
    cv::Mat frame = img;
    cv::putText(frame, "frame " + std::to_string(nFrame), cv::Point(int(200), int(100)),
                cv::FONT_HERSHEY_PLAIN, 3, cv::Scalar(255, 0, 0), 2, 8, 0);
    cv::imshow("video", frame);
    t = ((double)cv::getTickCount() - t) * 1000 / cv::getTickFrequency();
    qDebug() << " nFrame = " << nFrame << ", Show time for a frame : " << t << "ms.";
}


// 定时器到时处理槽函数
void QManager::onTimeOut()
{
    double t = (double)cv::getTickCount();
    qDebug()<< " QManager onTimeOut(), tid :" << QThread::currentThreadId();
    int nFrame = g_fNum;
    cv::Mat frame = g_img;
    cv::putText(frame, "frame " + std::to_string(nFrame), cv::Point(int(200), int(100)),
                cv::FONT_HERSHEY_PLAIN, 3, cv::Scalar(255, 0, 0), 2, 8, 0);
    cv::imshow("video", frame);
    t = ((double)cv::getTickCount() - t) * 1000 / cv::getTickFrequency();
    qDebug() << " nFrame = " << nFrame << ", Show time for a frame : " << t << "ms.";
}
