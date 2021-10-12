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

    return a.exec();
}
