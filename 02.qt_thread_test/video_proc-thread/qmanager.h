#ifndef QMANAGER_H
#define QMANAGER_H

#include <string>
#include <QTimer>
#include <QObject>
#include <QThread>
#include <opencv2/opencv.hpp>
#include "qreader.h"
#include "qprocess.h"


class QManager: public QObject
{

  Q_OBJECT

public:
    QManager(const std::string& url);

public:
    int run();

private:
    QReader m_reader;
    QProcess m_proc;

    cv::Mat m_frame;
    QTimer m_timer; // 定时器

    QThread* m_t1;
    QThread* m_t2;


public slots:
    void onTimeOut(); //定期器处理槽函数
    void onImageProcessed(cv::Mat &img, int ret);
};

#endif // QMANAGER_H
