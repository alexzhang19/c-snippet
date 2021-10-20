#ifndef QREADER_H
#define QREADER_H

#include <string>
#include <QThread>
#include <opencv2/opencv.hpp>


class QReader: public QThread
{
    Q_OBJECT

public:
    QReader(const std::string& url);
    void run();

    cv::Mat curImg(int &num)
    {
        num = m_Num;
        return m_frame;
    }

private:
    std::string m_url;
    cv::Mat m_frame;
    int m_Num;
};

#endif // QREADER_H
