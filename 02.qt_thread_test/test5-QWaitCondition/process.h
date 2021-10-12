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
    QThreadProducer threadProducer;
    QThreadConsumer threadConsumer;

public:
    void startThread();
    void stopThread();

private slots:
    void onthreadA_started();
    void onthreadA_finished();

    void onthreadB_started();
    void onthreadB_finished();

    void onthreadB_newValue(int seq, int diceValue);
};

#endif // PROCESS_H
