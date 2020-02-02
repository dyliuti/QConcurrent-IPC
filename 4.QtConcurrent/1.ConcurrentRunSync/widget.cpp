#include "widget.h"
#include "ui_widget.h"
#include <QtConcurrent>
#include <QThread>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    qDebug() << "Ui thread : " << QThread::currentThread();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::heavyWork()
{
    qDebug() << "Heavy work running in thread : " << QThread::currentThread();
    for(int i{0} ; i < 1000000001 ; i++){
        if((i%100000) == 0){
            double percentage = ((i/1000000000.0)) * 100;
                       qDebug() << "Percentage : " << QVariant::fromValue(percentage).toInt()
                                << " | Thread : " << QThread::currentThread();
        }
    }
}

void Widget::on_startButton_clicked()
{
   /* 在一个单独的线程中运行一个函数 */
   // future = QtConcurrent::run(heavyWork);
   /* 在线程池中运行一个函数 可以看到指向线程的指针地址很多不一样的 */
   QThreadPool pool;
   future = QtConcurrent::run(&pool, heavyWork);
   // 将会阻塞直到线程执行完，等于同步了
   future.waitForFinished();
   qDebug() << "Computation done!";
}

/* 有无future.waitForFinished()都没效果，因为是同步的，只有等线程中的执行完后才执行 */
void Widget::on_cancelButton_clicked()
{
    qDebug() << "Cancel......";
    future.cancel();
}

/* 有无future.waitForFinished()都没效果，因为是同步的，只有等线程中的执行完后才执行 */
void Widget::on_pauseButton_clicked()
{
    qDebug() << "Pause......";
    future.pause();
}

/* 有无future.waitForFinished()都没效果，因为是同步的，只有等线程中的执行完后才执行 */
void Widget::on_resumeButton_clicked()
{
    qDebug() << "Resume......";
    future.resume();
}
