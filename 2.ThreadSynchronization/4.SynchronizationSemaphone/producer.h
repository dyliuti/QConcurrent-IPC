#ifndef PRODUCER_H
#define PRODUCER_H

#include <QThread>
#include <QSemaphore>
#include <QVector>

class Producer : public QThread
{
    Q_OBJECT
public:
    explicit Producer(const QVector<int> & dataSource,
                      int * buffer,int bufferSize,QSemaphore * freeSpace,
                     QSemaphore * availableSpace,
                     bool * atEnd,QObject *parent = nullptr);

signals:

public slots:

protected:
    void run() override;

private:
    QVector<int> m_data_source;
    int * m_buffer;
    int m_BUFFER_SIZE;
    QSemaphore * m_free_space;// 生产者写入的缓存
    QSemaphore * m_available_space;// 消费者可读取的缓存
    bool * m_at_end;
};

#endif // PRODUCER_H
