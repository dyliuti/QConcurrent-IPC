**“纸上得来终觉浅，绝知此事要躬行。”  —— 陆游**

**"Practice，practice，practice and summary make perfect" —— dyliuti**

------



<br>

**简介：**

Qt中线程同步的方法。

<br>

**总结：**

**一：线程同步方法：**

互斥量、读写锁、信号量、条件变量。多个线程共享的数据，且需要修改的需要做同步保护。

<br>

**文件说明：**

1.SynchroQMutex：线程同步，使用QMutex或QMutexLocker。使用QMutex以及封装了QMutex的区域锁QMutexLocker。 

2.MutexSharedVariable：线程同步，使用互斥量QMutex保护进程中需要在不同线程中修改的共享数据。进程中三个PrintWorker线程共用PrintDevice对象中的数据m_count，需加锁保护。text只是引用读取，未多线程竞争修改，不需要保护。

3.SynchronizationReadWriteLock：线程同步，使用读写锁QReadWriteLock。当有着大量读取操作时，读写锁可以提升效率。因为获取、释放锁的过程较费时，而读取锁在读取时，只是原子变量引用计数+1，提升效率。

4.SynchronizationSemaphone：线程同步，使用多元信号量QSemaphore。生产者消费者模式，将生产者产生的缓存大小作为多元信号量的引用；将消费者可消费的缓存的大小也作为多元信号量的引用。信号量acquire()一次引用计数-1，若acquire()时信号量引用计数小于1就会阻塞在这里，直到其他线程有释放该信号量为止。release()一次引用计数+1。通过available()判断信号量引用计数是否大于0，可作为是否可继续消费的判断条件。

5.SynchronizationWaitConditions：线程同步，使用条件变量QWaitCondition与QMutex。与4.同逻辑，将信号量改为条件变量了。条件变量wait()会阻塞该线程成为等待状态，并释放所关联的锁，当别的线程进行wakeAll()或wakeOne()，该条件变量就有机会从等待状态回到正常状态，可继续执行下面代码。

6.WaitConditionPauseResume：线程同步，使用条件变量QWaitCondition与QMutex。利用子类化QThread除run外的构造、析构、成员函数都在主线程中执行的特点，通过bool变量与条件变量，达到主线程控制子线程是否继续运行的目的。