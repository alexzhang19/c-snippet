#ifndef THREADTEST_H
#define THREADTEST_H

#include <QThread>
#include <QDebug>

class ThreadTest: public QThread
{
    Q_OBJECT
public:
    ThreadTest()
    {
        connect(this, SIGNAL(counter(int)), this, SLOT(getCounter(int)));
        connect(this, SIGNAL(counter_reset()), this, SLOT(on_counter_reset()));
    }

    void run()
    {
        qDebug()<<"ThreadTest tid "<<currentThreadId();
        int m_counter = 0;
        for(int i=0; i<10; i++)
        {
            if(0==i%5)
            {
                m_counter = 0;
                emit counter_reset();
            }

            emit counter(m_counter);
            m_counter = m_counter + 1;
            msleep(200);
        }
    }

signals:
    void counter(int n);
    void counter_reset();

public slots:
    void getCounter(int n)
    {
        qDebug()<< " tid:"<<currentThreadId()<< ", getCounter : " <<n;
    }

    void on_counter_reset()
    {
        qDebug()<<" tid:"<<currentThreadId()<< "counter reset now !";
    }
};


#endif // THREADTEST_H
