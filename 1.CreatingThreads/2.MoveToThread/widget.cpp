#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "worker.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug() << "Widget Constructor running in GUI thread : " << thread();
    qDebug() << "Current thread in Widget constructor : " << QThread::currentThread();

    m_workerThread = new QThread(this);
    connect(m_workerThread,&QThread::finished,this,&Widget::threadFinished);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_startButton_clicked()
{
    Worker * worker = new Worker;//Main thread
    // moveToThread会让这个类及其子类都移动目标线程，并且事件循环在目标线程中继续。如QEvet::ThreadChange也要在目标对象所在的目标线程的事件循环中捕获
    // 此外要移动的类不能有除QObject之外的父类
    worker->moveToThread(m_workerThread);

    connect(m_workerThread,&QThread::started,worker,&Worker::doCounting);
    connect(worker,&Worker::currentCount,this,&Widget::currentCount);
    // 结束事件循环才能接着触发finished信号
    connect(worker,&Worker::countDone,this,&Widget::countDone);
    connect(m_workerThread,&QThread::finished,worker,&Worker::deleteLater);

    m_workerThread->start();
}

// 检测线程有没在运行
void Widget::on_infoButton_clicked()
{
    qDebug() << " Thread running : " << m_workerThread->isRunning();
}

void Widget::countDone()
{
    qDebug() << "Widget, count Done";
    // 结束事件循环
    m_workerThread->exit();
}

void Widget::currentCount(int value)
{
    ui->progressBar->setValue(value);
    ui->infoLabel->setText(QString::number(value));

}

void Widget::threadFinished()
{
    qDebug() << "Thread finished. Thread : " << thread();
    qDebug() << "Thread finished.Current Thread : " << QThread::currentThread();
}
