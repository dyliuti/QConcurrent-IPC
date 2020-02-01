**“纸上得来终觉浅，绝知此事要躬行。”  —— 陆游**

**"Practice，practice，practice and summary makes perfect" —— dyliuti**

------



<br>

**简介：**

Qt中线程的创建方法及其区别，以及各种创建线程方法时子线程向主线程发送反馈数据的方法和需要注意的地方。

<br>

**总结：**

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

<br>

**文件说明：**

1.QThreadCreate：使用QThread的静态函数[QThread](qthread.html#QThread) *QThread::create(Function &&*f*, Args &&... *args*)创建线程。函数指针Function可以是类成员函数、全局函数或lambda函数等。lambada中使用捕获的方式，可以轻松调用类成员函数等，而无需传入对象指针进行调用。 

**注意：**该方式创建的线程中无事件循环。

2.MoveToThread：使用QObject的成员方法void QObject::moveToThread([QThread](qthread.html) **targetThread*)创建线程。需要通过QThread::started信号链接到对应的槽函数中，该槽函数是在targetThread线程中执行的函数。

**注意：**该方式创建的线程自动带上事件循环。

3.SubclassQThread：子类化QThread，并重新实现run()方法。run()方法等同于1、2两例中的函数指针。除了run()外、构造、析构、成员函数都在主线程中运行。

**注意：**该方式创建的线程中事件循环需要程序员手动开启，通过exec()方法。

4.1AsyncQThreadCreate：使用1.QThreadCreate中 QThread::create方法创建线程。通过在线程函数中使用定时器，测试该方法创建的线程无事件循环。

4.2AsyncMoveToThread：使用2.MoveToThread中 moveToThread方法创建线程。通过在线程函数中使用定时器，测试该方法创建的线程自动带上事件循环。

4.3AsyncSubclassQThread：使用3.SubclassQThread中 子类化QThread方法创建线程。通过在线程函数中使用定时器，并通过是否执行exec()测试该方法创建的线程自动带上事件循环。

5.1ThreadPoolRunnable：通过线程池中分配出一个线程。子类化QRunnable，并实现纯虚函数run()。然后通过void QThreadPool::start([QRunnable](qrunnable.html) **runnable*, int *priority* = 0)，从线程池中分配出一个线程执行QRunnable子类中的run()函数。

5.2ThreadPoolFeedback：测试子类化QRunnable并实现run()，然后使用线程池中分配出一个线程来运行该run()方法时，要返回数据，哪些方法可行。这种方法中要想返回数据可通过如下两种方法：

一：在线程中通过QApplication::postEvent向目标对象发送事件。

二：通过元对象系统，调用QMetaObject::invokeMethod方法，指定Qt::QueuedConnection类型。

**注意：**这里不可以使用发送信号的方式，究其原因的话，QRunnable并非继承自QObject，也无Q_OBJECT宏，自然不能使用信号啦。而子类画QThread时都可以使用，你懂的。

5.3ThreadPoolAsync：子类化QRunnable并实现run()的方法也无事件循环。通过定时器测试。

6.CustomTypeSignals：也是工作线程向主线程发送数据，但此时发送的是自定义的类。在线程中通过emit发送信号，链接到主线程的槽函数，其实也是通过事件队列的方式。跨线程通过信号槽方式，要使数据传送成功，需要将自定义的类注册到元对象系统中（比如源码中执行这种方式时，需要将数据转换为QVariant等，就需要注册类型了，是转换到QVariant吗？）。qRegisterMetaType或Q_DECLARE_TYPEINFO都可以。而同一个线程中通过信号槽方式，传送自定义的类，是不需要注册到元对象系统中的。