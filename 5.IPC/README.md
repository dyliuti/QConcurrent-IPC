**“纸上得来终觉浅，绝知此事要躬行。”  —— 陆游**

**"Practice，practice，practice and summary make perfect" —— dyliuti**

------



<br>

**简介：**

Qt中进程间通信的方法。

<br>

**总结：**

**一：Qt进程间通信方式：**

1.通过QProcess可拉起、关闭其它进程。

2.通过对共享内存段的访问，QSharedMemory(只linux有效)提供了一种方法来锁定独占访问的内存 。

3.采用TCP网络通信的方式(个人认为在Windows中是最佳选择)。

4.采用文件作为中间媒介，进行通信。QFileSystemWatcher进行文件变动监测。

5.Dbus(只linux有效，个人认为在Linux中是最佳选择，连端口都不需要开，安全)。

<br>

**文件说明：**

1.QProcess：QProcess的start函数通过传入进程全路径名，可执行该进程。通过close可关闭该进程。

2.SharedMemory：QSharedMemory 提供对共享内存段的访问， 还提供了一种方法来锁定独占访问的内存 。 该类在Windows中没用，在linux中可用。访问共享内存时，需要进程先成功attach，然后再加锁读取或写入共享内存，操作完后进程需要detach。当是最后一个进程与共享内存段分离时， QSharedMemory将自动销毁共享内存段。 

3.TcpClient-TcpServer：采用网络通信的方式在本机两个进程间进行通信，要求就是服务端需要开端口。

4.FileProducer-FileConsumer：采用文件作为中间媒介，进行通信。QFileSystemWatcher可检测文件或文件夹的变动，进而得到重新读取文件的时间点，达到写读同步的目的。

