#include "consumer.h"
#include <QDebug>

Consumer::Consumer(char * buffer, int bufferSize,
                   int * usedSlots, int totalData,
                   QMutex * mutex,
                   QWaitCondition * bufferFull,QWaitCondition * bufferEmpty,
                   QObject *parent) : QThread(parent),
    m_buffer(buffer), m_buffer_size(bufferSize),
    m_used_slots(usedSlots),m_total_data(totalData),
    m_mutex(mutex),
    m_buffer_full(bufferFull),
    m_buffer_empty(bufferEmpty)
{

}

void Consumer::run(){
    for (int i = 0; i < m_total_data; ++i) {
        m_mutex->lock();
        if((*m_used_slots) == 0){
            /* 从锁定状态->等待状态 */
            /* 阻塞并释放m_mutex, 直到m_buffer_empty的wake通知来了再往下执行 */
            m_buffer_empty->wait(m_mutex);
        }
        m_mutex->unlock();

        // 写数据到控制台
        qDebug() << "Consumer data : " << m_buffer[i % m_buffer_size];

        m_mutex->lock();
        --(*m_used_slots);
        m_buffer_full->wakeAll();   // 不满时就通知
        m_mutex->unlock();
    }
}
