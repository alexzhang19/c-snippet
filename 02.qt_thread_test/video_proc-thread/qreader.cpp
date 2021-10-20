#include "qreader.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "g_base.h"

using namespace cv;


QReader::QReader(const std::string& url)
{
    m_url = url;
}


void QReader::run()
{
    qDebug()<< " QReader run(), tid :" << QThread::currentThreadId();

    cv::VideoCapture cap;
    cap.open(m_url);
    if(!cap.isOpened())
    {
        qDebug() << " ===> please check the video path.";
        return;
    }

    bool is_start = false;
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    while(true)
    {
        int frame_id = cap.get(cv::CAP_PROP_POS_FRAMES);
//        qDebug() << " frame_id =" << frame_id;

        g_m_reader.lock();
        if (is_start)
            g_wc_proc.wait(&g_m_reader);
        else
            is_start = true;

        qDebug()<< " QReader run(), tid :" << QThread::currentThreadId();
        double t = (double)cv::getTickCount();
        cap.read(g_img);
        if (g_img.empty())
        {
            qDebug() << " ===> image empty.";
            continue;
        }
        g_fNum += 1;
        t = ((double)cv::getTickCount() - t) * 1000 / cv::getTickFrequency();
        qDebug() << " g_fNum = " << g_fNum << ", Decode time for a frame : " << t << "ms";

        msleep(20);
        g_m_reader.unlock();
        g_wc_reader.wakeAll();
    }
    cap.release();

    quit(); // 相当于exit(0),退出线程的事件循环
}
