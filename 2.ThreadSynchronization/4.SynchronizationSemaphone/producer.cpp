#include "producer.h"

Producer::Producer(const QVector<int> & dataSource,  int * buffer,
                   int bufferSize,QSemaphore *freeSpace,
                   QSemaphore *availableSpace, bool *atEnd,QObject *parent) :
    QThread(parent),
    m_data_source(dataSource),
    m_buffer(buffer),
    m_BUFFER_SIZE(bufferSize),
    m_free_space(freeSpace),
    m_available_space(availableSpace),
    m_at_end(atEnd)
{

}

void Producer::run()
{
    for(int i{0} ; i < m_data_source.length() ; i++){
        /* 空置数据信号量引用计数不为0，就-1，若为0就在这堵塞 */
        m_free_space->acquire();
        /* 写数据 */
        m_buffer[ i % m_BUFFER_SIZE] = m_data_source.at(i);

        if(i == m_data_source.length() -1)
            *m_at_end = true;

        /* 数据可用信号量 引用计数+1 */
        m_available_space->release();
    }

}
