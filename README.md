**“纸上得来终觉浅，绝知此事要躬行。”  —— 陆游**

**"Practice，practice，practice and summary makes perfect" —— dyliuti**

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

**二：几种创建线程方式的区别：**

1.QThread::create：线程中无事件循环。

2.QObject::moveToThread：线程自动带上事件循环。

3.子类化QThread：通过有没执行exec()来决定是否开启事件循环。

**三：线程中数据反馈到主线程的几种方法：**

1.在线程中通过QApplication::postEvent向目标对象发送事件。

2.通过元对象系统，调用QMetaObject::invokeMethod方法，指定Qt::QueuedConnection类型。

3.通过信号与槽的连接。

说明:明显1.方法目标对象得开启事件循环。2.方法目标类得是继承于QObject且有Q_OBJECT宏。3.方法除了满足上述12条件外，还需线程所在的类也是继承于QObject且有Q_OBJECT宏。

**四：自定义类型通过信号与槽跨线程传送，需要先将该类型注册到元对象系统。**

