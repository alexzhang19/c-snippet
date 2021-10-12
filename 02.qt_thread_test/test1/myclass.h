#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>
#include <QDebug>
#include <QThread>

class MyClass : public QObject
{
    Q_OBJECT
public:
    MyClass(){}

public slots:
    void getstarted()
    {
        qDebug()<<objectName()<<":"<<"getstarted() , tid :"<<QThread::currentThreadId();
    }

    void getfinished()
    {
        qDebug()<<objectName()<<":"<<"getfinished(), tid :"<<QThread::currentThreadId();
    }
};

#endif // MYCLASS_H
