#include "process.h"
#include <QDebug>
#include <QThread>
//#include <unistd.h>

Process::Process()
{
    connect(&threadA, SIGNAL(started()), this, SLOT(onthreadA_started()));
    connect(&threadA, SIGNAL(finished()), this, SLOT(onthreadA_finished()));

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    qDebug()<< " Process Process(), tid :" << QThread::currentThreadId();
}

// 启动线程
void Process::startThread()
{
    qDebug()<< " Process startThread(), tid :" << QThread::currentThreadId();

    threadA.moveToThread(&threadA);
    threadA.diceBegin(); // 控制变量，实现线程循环开始
    threadA.start(); // 线程开启，执行run()虚函数

    mSeq=0;
    mTimer.start(100); //定时器100读取一次数据
}

// 停止线程
void Process::stopThread()
{
    qDebug()<< " Process stopThread(), tid :" << QThread::currentThreadId();
    if (threadA.isRunning())
    {
        threadA.stopThread(); // 控制变量，实现线程循环结束
        threadA.wait();// 等待线程结束
    }

    mTimer.stop(); // 定时器暂停
}

void Process::onthreadA_started()
{
    qDebug()<< " Process onthreadA_started(), tid :" << QThread::currentThreadId();
}

void Process::onthreadA_finished()
{
    qDebug()<< " Process onthreadA_finished(), tid :" << QThread::currentThreadId();
}

// 定时器到时处理槽函数
void Process::onTimeOut()
{
    qDebug()<< " Process onTimeOut(), tid :" << QThread::currentThreadId();
    int tmpSeq=0, tmpValue=0;
    bool valid = threadA.readValue(&tmpSeq, &tmpValue); //读取数值
    if (valid && (tmpSeq != mSeq)) //有效，并且是新数据
    {
        mSeq = tmpSeq;
        mDiceValue = tmpValue;
        qDebug()<< " get new value :" << "mSeq=" << mSeq << ",mDiceValue=" << mDiceValue;
    }

    // usleep(1000);
}
