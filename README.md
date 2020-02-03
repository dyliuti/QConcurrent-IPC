**“纸上得来终觉浅，绝知此事要躬行。”  —— 陆游**

**"Practice，practice，practice and summary make perfect" —— dyliuti**

------



<br>

**简介：**

Qt线程API测试与总结，Qt跨进程通信API测试与总结。

每个文件夹中都有说明文档，说明每个工程在干什么，以及对于这个文件夹中内容的总结。

<br>

**各文件夹说明文档总结汇总：**

## 1.CreatingThreads

**一：线程销毁方法：**

connect(thread,&QThread::finished,thread,&QThread::deleteLater);

1.在QCoreApplication::exec()执行前调用deleteLater，一旦调用该函数，该类会被销毁（不适用线程类情况）。在主事件循环停止后调用deleteLater，则不会删除该对象。

2.如果在没有事件循环的线程中调用deleteLater，则在线程结束时销毁该对象。

3.在有事件循环的线程中调用deleteLater，则在线程中deleteLater对应事件前的所有pending events处理完后销毁该对象。

**二：几种创建线程方式的区别：**

1.QThread::create：线程中无事件循环。

2.QObject::moveToThread：线程自动带上事件循环。

3.子类化QThread：通过有没执行exec()来决定是否开启事件循环。

4.通过线程池中分配出一个线程。子类化QRunnable，并实现纯虚函数run()。无事件循环。

**三：线程中数据反馈到主线程的几种方法：**

1.在线程中通过QApplication::postEvent向目标对象发送事件。

2.通过元对象系统，调用QMetaObject::invokeMethod方法，指定Qt::QueuedConnection类型。

3.通过信号与槽的连接。

说明:明显1.方法目标对象得开启事件循环。2.方法目标类得是继承于QObject且有Q_OBJECT宏。3.方法除了满足上述12条件外，还需线程所在的类也是继承于QObject且有Q_OBJECT宏。

**四：自定义类型通过信号与槽跨线程传送，需要先将该类型注册到元对象系统。**

## 2.ThreadSynchronization

**一：线程同步方法：**

互斥量、读写锁、信号量、条件变量。多个线程共享的数据，且需要修改的需要做同步保护。

## 3.ThreadSafetyReentrancy

**一：线程反馈数据：**

1.跨线程信号与槽连接要在发送数据前就连上，不然会丢失数据。

2.基于事件的类应该只在一个线程中调用。如QTimer。

## 4.QtConcurrent

**一：QtConcurrent::run：**

1.QtConcurrent::run()返回的QFuture不支持取消、暂停或进度报告。返回的QFuture只能用于查询函数的运行/完成状态和返回值。 

2.QFutureWatcher::cancel并不是所有的异步计算都可以取消。例如，QtConcurrent::run()返回的QFuture不能被取消;但是由QtConcurrent::mappedReduced()返回的QFuture可以。

**二：QtConcurrent::map、mapped、mappedReduce、filter等：**

1.mapReduced中的reduceFunction中的结果可自定，是模板参数T，此时future的result()与results()分别对应T与QLIst< T>。

2.QtConcurrent中不带ed的函数，如map返回的是future<void>，模板参数是固定了，将结果都转换到void类型。而带ed的如mapped，mappedReduced是和函数中返回值类型或引用值类型相同的。  

3.这些函数都可同步等待他们完成或异步执行，可参考下面具体实例。

**三：QFutureWatcher的progressValueChanged信号发射速率很低，主要是为了避免GUI事件循环超载。但结束的进度值肯定是会发送的。**

## 5.IPC

**一：Qt进程间通信方式：**

1.通过QProcess可拉起、关闭其它进程。

2.通过对共享内存段的访问，QSharedMemory(只linux有效)提供了一种方法来锁定独占访问的内存 。

3.采用TCP网络通信的方式(个人认为在Windows中是最佳选择)。

4.采用文件作为中间媒介，进行通信。QFileSystemWatcher进行文件变动监测。

5.Dbus(只linux有效，个人认为在Linux中是最佳选择，连端口都不需要开，安全)。