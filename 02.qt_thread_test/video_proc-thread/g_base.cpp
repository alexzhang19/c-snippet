#include "g_base.h"

// threadDAQ
cv::Mat g_img; // 全局图像
int g_fNum = 0; // 全局图像帧计数器


QMutex g_m_reader;
QWaitCondition g_wc_reader;


QMutex g_m_proc;
QWaitCondition g_wc_proc;
