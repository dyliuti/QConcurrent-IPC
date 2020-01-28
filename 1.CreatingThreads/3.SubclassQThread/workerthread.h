#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QTimer>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = nullptr);
    ~WorkerThread() override;

signals:
    void currentCount(int value);

private:
    QTimer * m_timer;
public slots:

    // QThread interface
protected:
    void run() override;
};

#endif // WORKERTHREAD_H
