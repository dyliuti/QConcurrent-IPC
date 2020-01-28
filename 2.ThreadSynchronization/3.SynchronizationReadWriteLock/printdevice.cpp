#include "printdevice.h"
#include <QDebug>

PrintDevice::PrintDevice()
{
    m_count = 0;
}


void PrintDevice::print(const QString  & text){
    // 使用读写锁，提升效率
    QReadLocker readLocker(&m_read_write_lock);
    qDebug() << text << "Called , count : " << m_count ;
}

void PrintDevice::increment(){
    QWriteLocker writeLocker(&m_read_write_lock);
    m_count++;
}
