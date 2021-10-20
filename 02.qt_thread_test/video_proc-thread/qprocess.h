#ifndef QPROCESS_H
#define QPROCESS_H

#include <string>
#include <QThread>
#include <opencv2/opencv.hpp>


class QProcess: public QThread
{
    Q_OBJECT

public:
    QProcess();
    void run();

private:
    int ret;

signals:
    void processed(cv::Mat &img, int ret);

};

#endif // QPROCESS_H
