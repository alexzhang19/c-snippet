#ifndef G_BASE_H
#define G_BASE_H

#include <QMutex>
#include <QWaitCondition>
#include <opencv2/opencv.hpp>

extern cv::Mat g_img; // 全局图像
extern int g_fNum; // 全局图像帧计数器


extern QMutex g_m_reader;
extern QWaitCondition g_wc_reader;


extern QMutex g_m_proc;
extern QWaitCondition g_wc_proc;


#endif // G_BASE_H
