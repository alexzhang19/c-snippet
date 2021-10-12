#include "process.h"
#include <QDebug>
#include <QThread>

Process::Process()
{
    connect(&threadA, SIGNAL(started()), this, SLOT(onthreadA_started()));
    connect(&threadA, SIGNAL(finished()), this, SLOT(onthreadA_finished()));
    connect(&threadA, SIGNAL(newValue(int,int)), this, SLOT(onthreadA_newValue(int,int)));
    qDebug()<<  "Process Process(), tid :" << QThread::currentThreadId();
}

//Process::Process(QObject *parent):QObject(parent)
//{
//    connect(&threadA, SIGNAL(started()), this, SLOT(onthreadA_started()));
//    connect(&threadA, SIGNAL(finished()), this, SLOT(onthreadA_finished()));
//    connect(&threadA, SIGNAL(newValue(int,int)), this, SLOT(onthreadA_newValue(int,int)));
//}

// 启动线程
void Process::startThread()
{
    qDebug()<< " Process startThread(), tid : "<< QThread::currentThreadId();
    threadA.moveToThread(&threadA);
    threadA.diceBegin(); // 控制变量，实现线程循环开始
    threadA.start(); // 线程开启，执行run()虚函数
}

// 停止线程
void Process::stopThread()
{
    qDebug()<<  " Process stopThread(), tid : "<< QThread::currentThreadId();
    if (threadA.isRunning())
    {
        threadA.stopThread(); // 控制变量，实现线程循环结束
        threadA.wait();// 等待线程结束
    }
}

void Process::onthreadA_started()
{
    qDebug()<<  "Process onthreadA_started(), tid : " << QThread::currentThreadId();
}

void Process::onthreadA_finished()
{
    qDebug()<<  "Process onthreadA_finished(), tid : " << QThread::currentThreadId();
}

void Process::onthreadA_newValue(int seq, int diceValue)
{
    qDebug()<<  "Process onthreadA_newValue(), tid : " << QThread::currentThreadId();
    qDebug() << "get value: seq=" << seq << ", diceValue=" << diceValue;
   // sleep(1);
    if (seq > 3)
    {
        stopThread();
    }
}
