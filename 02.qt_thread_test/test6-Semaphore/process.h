#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QTimer>
#include "qmythread.h"

class Process: public QObject
{
    Q_OBJECT

public:
    Process();

private:
    QThreadDAQ threadProducer;
    QThreadShow threadConsumer;

public:
    void startThread();
    void stopThread();

private slots:
    void onthreadA_started();
    void onthreadA_finished();

    void onthreadB_started();
    void onthreadB_finished();

//    void onthreadB_newValue(int seq, int diceValue);
    void onthreadB_newValue(int *data, int count, int bufNo);
};

#endif // PROCESS_H
