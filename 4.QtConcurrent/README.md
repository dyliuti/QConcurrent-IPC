**“纸上得来终觉浅，绝知此事要躬行。”  —— 陆游**

**"Practice，practice，practice and summary make perfect" —— dyliuti**

------



<br>

**简介：**

QtConcurrent使快速编写Qt多线程程序成为可能，而无需使用低级线程类库。

<br>

**总结：**

**一：QtConcurrent::run：**

1.QtConcurrent::run()返回的QFuture不支持取消、暂停或进度报告。返回的QFuture只能用于查询函数的运行/完成状态和返回值。 

2.QFutureWatcher::cancel并不是所有的异步计算都可以取消。例如，QtConcurrent::run()返回的QFuture不能被取消;但是由QtConcurrent::mappedReduced()返回的QFuture可以。

**一：QtConcurrent::map、mapped、mappedReduce等：**

1.mapReduced中的reduceFunction中的结果可自定，是模板参数T，此时future的result()与results()分别对应T与QLIst< T>。

2.QtConcurrent中不带ed的函数，如map返回的是future<void>，模板参数是固定了，将结果都转换到void类型。而带ed的如mapped，mappedReduced是和函数中返回值类型或引用值类型相同的。  

3.这些函数都可同步等待他们完成或异步执行，可参考下面具体实例。

<br>

**文件说明：**

1.ConcurrentRunSync：QtConcurrent有两个run函数，分别是run(Function *function*, *...*)和run(QThreadPool **pool*, Function *function*, *...*)，前者单线程，后者从线程池中取一个线程执行function函数中的任务，本质都是单线程的。run()函数返回值都是QFuture< T>，  T与function函数的返回值是同一类型的  。waitForFinished用于等待异步计算完成，即程序会阻塞于此。

**注意：**QtConcurrent::run()返回的QFuture不支持取消、暂停或进度报告。返回的QFuture只能用于查询函数的运行/完成状态和返回值。 

2.ConcurrentRunAsyc：还是使用QtConcurrent的run函数返回QFuture，然后使用QFutureWatcher通过sefFuture关联QFuture，QFutureWatcher的作用是检测工作进度。此时QFutureWatcher的cancel函数不起作用。执行pause函数的作用不能阻止线程接口函数里的内容执行完，但可做到执行完后不会发finished等信号。而resume函数就可以使其继续往下执行，如发送finished信号。

**注意：** QFutureWatcher::cancel并不是所有的异步计算都可以取消。例如，QtConcurrent::run()返回的QFuture不能被取消;但是由QtConcurrent::mappedReduced()返回的QFuture可以。 

3.ConcurrentMap：QFutureWatcher的map(Sequence &*sequence*, MapFunctor *function*)函数返回QFuture，然后使用QFutureWatcher通过sefFuture关联QFuture。用例中MapFunctor 用于直接修改序列中的值，该过程是多线程并发进行的，观察打印出的序列号和线程地址可得出。

**注意：**QFutureWatcher的progressValueChanged信号发射速率很低，主要是为了避免GUI事件循环超载。但结束的进度值肯定是会发送的。

4.ConcurrentMapped：与3.基本一样，除了把map函数换成mapped函数，两者的区别在于后者可通过future.resultCount()返回包含修改内容的新序列，对原序列不做改动，而前者map则更改原序列值的。

5.ConcurrentMapReduced：QtConcurrent::mappedReduced(const Sequence &*sequence*, MapFunctor *mapFunction*, ReduceFunctor *reduceFunction*, QtConcurrent::ReduceOptions *reduceOptions* = ReduceOptions(UnorderedReduce | SequentialReduce))。mapFunction是多线程的接口函数，由多个线程调用，必须线程安全，传入参数为sequence的每一项。每个mapFunction的返回值被传递给reduceFunction，reduceFunction将每个线程接口mapFunction的返回值添加到最终结果，该结果通过future.result()可得到。所以reduceFunction参数有两个，result(通过future.result()可得到)和w(mapFunction返回值)。这个函数一次只能被一个线程调用，调用reduceFunction的顺序由reduceOptions决定。

**注意：**mappedReduced可同步，用waitForFinished 阻塞UI；也可异步，通过信号槽队列链接方式，链接&QFutureWatcher<void>::finished信号，不阻塞UI。注释了的代码就是同步的。mappedReduce效率比单线程高，验证结果如下：

![mappedReduced与单线程比较](https://github.com/dyliuti/QConcurrent-IPC/blob/master/%E5%9B%BE%E7%89%87/mappedReduced.png)

6.CuncurrentFilter：QtConcurrent::filter(Sequence &*sequence*, KeepFunctor *filterFunction*)遍历sequence，在原sequence中从头到尾保存符合条件的值。filterFunction中的参数为sequence的一项，函数作用就是比较sequence的一项与对应位置原序列中的一项谁大谁小。

7.ConcurrentFiltered：QtConcurrent::filtered(Sequence &*sequence*, KeepFunctor *filterFunction*)遍历原sequence，在新的sequence中保存符合条件的值。

8.ConcurrentFilterReduce：QtConcurrent::filteredReduced(const Sequence &*sequence*, KeepFunctor *filterFunction*, ReduceFunctor *reduceFunction*, QtConcurrent::ReduceOptions *reduceOptions* = ReduceOptions(UnorderedReduce | SequentialReduce))。与mappedReduced类似，filterFunction是多线程的接口函数，由多个线程调用，必须线程安全。每个filterFunction的**返回值true所对应的值**被传递给reduceFunction，reduceFunction将每个线程接口filterFunction的返回值添加到最终结果，该结果通过future.result()可得到。

9.FutureSynchronizer：QFutureSynchronizer可添加多个future，同时监测他们的工作完成情况。通过调用waitForFinished()可等这些被观测的future都工作完成后才往下执行。

