#include <QThread>
#include <QDebug>
#include "consumer.h"

Consumer::Consumer(QObject *parent) : QObject(parent)
{
    m_counter = 0;
}

void Consumer::data(const CustomData &cd)
{
    qDebug() << "Consumer message : " << cd.m_integer << *cd.m_pointer << cd.m_string
                 << " thread : " << QThread::currentThread();

    //停掉线程
    if( ++m_counter > 10){
        qDebug() << "Consumer , threat about to be killed";
        thread()->quit();
    }
}
