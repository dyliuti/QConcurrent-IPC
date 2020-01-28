#include "thread.h"
#include "consumer.h"

Thread::Thread(Producer * producer , QObject *parent) : QThread(parent),
    m_producer(producer)
{

}

void Thread::run()
{
    Consumer consumer;
    connect(m_producer, &Producer::data, &consumer, &Consumer::data);

    //开启事件循环，因为发送者是通过定时器到时后发送信号的
    //没事件循环的话，从Producer派送过来的消息分发不了到Consumer，没Event的dispatch？跨线程传数据得依赖事件循环
    exec();
}
