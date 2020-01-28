#include <QApplication>
#include <QMessageBox>
#include "printdevice.h"
#include "printworker.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool stopFlag = false;

    PrintDevice printDevice;

    /* PrintDevice对象就一个， 传入Worker类中，mutex起线程同步作用
     * 如果只有两个PrintWorker时会挨着输出（获取锁释放锁有时间限制，导致这个同步假象），三个时就会乱序了。
    */
    PrintWorker black("Black", &stopFlag, &printDevice);
    PrintWorker white("White", &stopFlag, &printDevice);
    PrintWorker blue("Blue", &stopFlag, &printDevice);

    /* 三个线程使用同一个类中的函数,同步的是m_count（线程共享资源），不是text（这个在不同线程的栈中） */
    black.start();
    white.start();
    blue.start();

    QMessageBox::information(nullptr,"QMutex",
                                 "Thread working. Close Me to stop");

    stopFlag = true;

    black.wait();
    white.wait();
    blue.wait();

    return 0;
}
