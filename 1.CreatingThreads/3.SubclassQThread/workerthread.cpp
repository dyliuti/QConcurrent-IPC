#include "workerthread.h"
#include <QDebug>

/* 主线程 */
WorkerThread::WorkerThread(QObject *parent) : QThread(parent)
{
    qDebug() << "Worker thread constructor running in thread : " << QThread::currentThread();
}

/* 主线程 */
WorkerThread::~WorkerThread()
{
    qDebug() << "Worker thread destructor running in thread : " << QThread::currentThread();
}

void WorkerThread::run()
{
    qDebug() << "Run method of thread running in thread : " << QThread::currentThread();

    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, [](){qDebug() << "timeout come in";});
    m_timer->start(1000);
    // thread()->quit();
    for(int i{0} ; i < 1000000001 ; i++){
        // 没100000次循环发送次信号
        if((i%100000) == 0){
            double percentage = ((i/1000000000.0)) * 100;
            emit currentCount(QVariant::fromValue(percentage).toInt());
        }
    }

    //不开启事件循环也没事，currentCount不是基于事件的函数，像QTimer::timerout就是基于事件循环的，不开启则发不出去
    //exec() ;
}
