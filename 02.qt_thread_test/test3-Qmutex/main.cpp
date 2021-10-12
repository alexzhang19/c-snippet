#include <QCoreApplication>
#include "qdice_thread.h"
//#include <unistd.h>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <iostream>
#include "process.h"
using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug()<< " mian(), tid :" << QThread::currentThreadId();
    Process proc;

    // 启动线程
    proc.startThread();

//    sleep(5);

//    // 结束线程
//    if (proc.m_recvCnt > 10)
//    {
//        proc.stopThread();
//    }

    return a.exec();
}
