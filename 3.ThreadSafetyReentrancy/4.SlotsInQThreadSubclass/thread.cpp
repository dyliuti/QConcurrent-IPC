#include "thread.h"
#include <QDebug>

Thread::Thread(QObject *parent) : QThread(parent),
    m_count(0)
{
    qDebug()  << "Thread constructor on thread : " << QThread::currentThread();
}

// 主线程中
void Thread::increment()
{
    qDebug() << "Increment called on thread : " << QThread::currentThread();
    m_count++;
    qDebug() << "m_count : " << m_count;

    if(m_count == 5){
        // 不能用m_timer，他不是存在于主线程中的
        // 基于事件的类应该只在一个线程中用
        //m_timer->stop();
        qDebug() << thread()->currentThread();
        /* 告诉线程退出事件循环，崩溃 */
        // thread()->quit();
    }
}

void Thread::run() {
    qDebug() << "Run called on thread : " << QThread::currentThread();
    /* m_timer = new QTimer(this);不要这样写，父子对象不在同一个线程中，有问题的 */
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &Thread::increment);
    m_timer->start(1000);
    // 若没事件循环，QTimer的timerout信号就发不出不去
    exec();
}
