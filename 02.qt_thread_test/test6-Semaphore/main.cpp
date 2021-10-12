#include <QCoreApplication>
#include <QDebug>
#include <QThread>
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
