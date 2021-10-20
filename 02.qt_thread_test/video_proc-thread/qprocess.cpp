#include "qprocess.h"
#include "g_base.h"
#include <QDebug>


QProcess::QProcess()
{

}


void QProcess::run()
{
    qDebug()<< " QProcess run(), tid :" << QThread::currentThreadId();

    int nFrame = 0;
    while(true)
    {
        g_m_proc.lock();
        g_wc_reader.wait(&g_m_proc);

        nFrame += 1;
        qDebug()<< " QProcess run(), tid :" << QThread::currentThreadId();
        msleep(1000);
        qDebug()<< " process nFrame = " << nFrame;
        emit processed(g_img, nFrame); // 此处未做线程同步，需深拷贝数据

        g_m_proc.unlock();
        g_wc_proc.wakeAll();
    }

    quit(); // 相当于exit(0),退出线程的事件循环
}
