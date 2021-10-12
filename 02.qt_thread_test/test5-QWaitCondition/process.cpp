#include "process.h"
#include <QDebug>
#include <QThread>
//#include <unistd.h>

Process::Process()
{
    connect(&threadProducer,SIGNAL(started()),this,SLOT(onthreadA_started()));
    connect(&threadProducer,SIGNAL(finished()),this,SLOT(onthreadA_finished()));

    connect(&threadConsumer,SIGNAL(started()),this,SLOT(onthreadB_started()));
    connect(&threadConsumer,SIGNAL(finished()),this,SLOT(onthreadB_finished()));

    connect(&threadConsumer,SIGNAL(newValue(int,int)),this,SLOT(onthreadB_newValue(int,int)));

    qDebug()<< " Process Process(), tid :" << QThread::currentThreadId();
}

// 启动线程
void Process::startThread()
{
    qDebug()<< " Process startThread(), tid :" << QThread::currentThreadId();
    threadConsumer.start();
    threadProducer.start();
}

// 停止线程
void Process::stopThread()
{
    qDebug()<< " Process stopThread(), tid :" << QThread::currentThreadId();
    if (threadProducer.isRunning())
    {
        threadProducer.stopThread();//结束线程的run()函数执行
        threadProducer.wait();//
    }

    if (threadConsumer.isRunning())
    {
        threadConsumer.terminate(); //因为threadB可能处于等待状态，所以用terminate强制结束
        threadConsumer.wait();//
    }
}

void Process::onthreadA_started()
{
    qDebug()<< " Process onthreadA_started(), tid :" << QThread::currentThreadId();
}

void Process::onthreadA_finished()
{
    qDebug()<< " Process onthreadA_finished(), tid :" << QThread::currentThreadId();
}

void Process::onthreadB_started()
{
    qDebug()<< " Process onthreadB_started(), tid :" << QThread::currentThreadId();
}

void Process::onthreadB_finished()
{
    qDebug()<< " Process onthreadB_finished(), tid :" << QThread::currentThreadId();
}

void Process::onthreadB_newValue(int seq, int diceValue)
{
    qDebug()<< " Process onthreadB_newValue(), tid :" << QThread::currentThreadId();

    qDebug()<< "onthreadB_newValue(), " << "seq=" << seq
            << "diceValue=" << diceValue;
}


