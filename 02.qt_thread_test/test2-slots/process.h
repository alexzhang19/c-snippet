#ifndef PROCESS_H
#define PROCESS_H


#include <QObject>
#include "qdice_thread.h"

class Process: public QObject
{
    Q_OBJECT

public:
    Process();
//    explicit Process(QObject *parent = nullptr);

private:
    QDiceThread threadA;

public:
    void startThread(); // 启动线程
    void stopThread(); // 停止线程

public slots:
    void onthreadA_started();
    void onthreadA_finished();
    void onthreadA_newValue(int seq, int diceValue);
};

#endif // PROCESS_H
