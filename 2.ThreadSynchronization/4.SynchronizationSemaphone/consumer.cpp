#include "consumer.h"
#include <QDebug>

Consumer::Consumer(int *buffer, int bufferSize,QSemaphore * freeSpace,
                   QSemaphore * availableSpace,
                   bool * atEnd,QObject *parent) :
    QThread(parent),
    m_buffer(buffer), m_BUFFER_SIZE(bufferSize),
    m_free_space(freeSpace),
    m_available_space(availableSpace),
    m_at_end(atEnd)
{

}

void Consumer::run()
{
    int i{0};
    while(!(*m_at_end) || m_available_space->available()){
        /* 若数据可用信号量引用计数不为0 数据可用信号量引用计数-1；若若数据可用信号量引用计数为0，这里阻塞 */
        m_available_space->acquire();

        /* 读数据 */
        qDebug() << "[" << i << "]" << m_buffer[i]; // 消费数据
        i =((i+1) % (m_BUFFER_SIZE));

        /* 空置数据信号量引用计数+1 */
        m_free_space->release();
    }
}
