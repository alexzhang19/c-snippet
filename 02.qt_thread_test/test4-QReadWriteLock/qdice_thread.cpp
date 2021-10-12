#include "qdice_thread.h"
#include <QTime>
#include <QDebug>
#include <QThread>

QDiceThread::QDiceThread():m_Paused(false)
{

}

void QDiceThread::diceBegin()
{
    // 开始掷骰子
    // mutex.lock();
    m_Paused=false;
    // mutex.unlock();

    qDebug()<< " QDiceThread diceBegin(), tid :" << QThread::currentThreadId();
}

void QDiceThread::dicePause()
{
    //暂停掷骰子
    // mutex.lock();
    m_Paused=true;
    // mutex.unlock();

    qDebug()<< " QDiceThread dicePause(), tid :" << QThread::currentThreadId();
}

void QDiceThread::stopThread()
{
    //停止线程
    // mutex.lock();
    m_stop=true;
    // mutex.unlock();

    qDebug()<< " QDiceThread stopThread(), tid :" << QThread::currentThreadId();
}

bool QDiceThread::readValue(int *seq, int *diceValue)
{
    if (mutex.tryLock())
    {
        *seq=m_seq;
        *diceValue=m_diceValue;
        mutex.unlock();
        return true;
    }
    else
        return false;
}

void QDiceThread::run()
{
     qDebug()<< " QDiceThread run(), tid :" << QThread::currentThreadId();

    //线程任务
    m_stop=false;//启动线程时令m_stop=false
    m_seq=0; //掷骰子次数
    qsrand(QTime::currentTime().msec());//随机数初始化，qsrand是线程安全的

    while(!m_stop)//循环主体
    {
        if (!m_Paused)
        {
            mutex.lock();
            m_diceValue=qrand(); //获取随机数
            m_diceValue=(m_diceValue % 6)+1;
            m_seq++;
            qDebug() << " produce new value: " << m_seq << m_diceValue
                     << "tid :" << QThread::currentThreadId();;
            mutex.unlock();
        }
        msleep(500); //线程休眠500ms
    }

//  在  m_stop==true时结束线程任务
    quit();//相当于  exit(0),退出线程的事件循环
}
