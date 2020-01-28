#include "producer.h"
#include <QDebug>
#include <QThread>

Producer::Producer(QWidget *parent) : QPushButton(parent)
{
    connect(this,&Producer::clicked,this,&Producer::sendData);
    m_integer = 10;
    setText("Produce");
}

void Producer::sendData()
{
    qDebug() << "Producer producing data , thread :" << QThread::currentThread();
    CustomData cd(10, &m_integer, "Produced data");
    // 发送定制的数据，将多个数据和为1个
    emit data(cd);
}
