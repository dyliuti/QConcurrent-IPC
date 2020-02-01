#ifndef CONSUMER_H
#define CONSUMER_H

#include <QThread>
#include <QSemaphore>

class Consumer : public QThread
{
    Q_OBJECT
public:
    explicit Consumer(int *buffer, int bufferSize,QSemaphore * freeSpace,
                      QSemaphore * availableSpace,
                      bool * atEnd,QObject *parent = nullptr);

signals:

public slots:

protected:
    void run() override;

private:
    int * m_buffer;
    int m_BUFFER_SIZE;
    QSemaphore * m_free_space;// 生产者写入的缓存
    QSemaphore * m_available_space;// 消费者可读取的缓存
    bool * m_at_end;
};

#endif // CONSUMER_H
