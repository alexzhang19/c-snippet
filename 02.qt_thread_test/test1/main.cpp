#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QThread>
#include "myclass.h"
#include "threadtest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<< "main tid: " << QThread::currentThreadId();
    ThreadTest threada;
    MyClass my;
    my.setObjectName("myClass");

    QObject::connect(&threada,SIGNAL(started()),&my,SLOT(getstarted()));
    QObject::connect(&threada,SIGNAL(finished()),&my,SLOT(getfinished()));

    // 改变对象依附
    threada.moveToThread(&threada);
    my.moveToThread(&threada);

    threada.start();
    qDebug()<< "main end";
    return a.exec();
}
