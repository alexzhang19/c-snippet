#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QTimer>
#include "qdice_thread.h"

class Process: public QObject
{
    Q_OBJECT

public:
    Process();

private:
    QDiceThread threadA;
    QTimer mTimer; // 定时器
    int mSeq, mDiceValue;

public:
    void startThread(); // 启动线程
    void stopThread(); // 停止线程

public slots:
    void onthreadA_started();
    void onthreadA_finished();

    void onTimeOut(); //定期器处理槽函数
};

#endif // PROCESS_H
