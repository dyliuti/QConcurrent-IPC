#include "worker.h"
#include <QDebug>
#include <QThread>
#include <QApplication>
#include <QMetaObject>
#include <QTimer>
#include <QVariant>
#include "progressevent.h"

Worker::Worker(QObject * receiver) : m_receiver(receiver)
{
    qDebug() << "Worker constructor running in thread : " << QThread::currentThread();
}

Worker::~Worker(){
    qDebug() << "Worker destructor running in thread : " << QThread::currentThread();
}

void Worker::run()
{
    qDebug() << "Worker run method running in thread : " << QThread::currentThread();

    /* 因为这里无事件循环，所以不能执行一些异步的代码，如QTimer、TCPSocket，如果非得执行了，等于白干 */
    QTimer * timer = new QTimer();
        QObject::connect(timer,&QTimer::timeout,[](){
            qDebug() << "Time out . Running in thread : " << QThread::currentThread();
        });
        timer->setInterval(1000);
        timer->start();
}
