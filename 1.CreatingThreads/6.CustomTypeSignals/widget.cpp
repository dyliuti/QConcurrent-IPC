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

    workerThread = new QThread(this);
    connect(workerThread,&QThread::finished,this,&Widget::threadFinished);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_startButton_clicked()
{

    Worker * worker = new Worker;//主线程

    worker->moveToThread(workerThread);

    connect(workerThread,&QThread::started,worker,&Worker::doCounting);
    connect(worker,&Worker::currentNumberString,this,&Widget::currentNumberString, Qt::QueuedConnection); // Qt::QueuedConnection 为什么这里不用这个
    connect(worker,&Worker::countDone,this,&Widget::countDone);
    connect(workerThread,&QThread::finished,worker,&Worker::deleteLater);

    workerThread->start();
}

void Widget::on_infoButton_clicked()
{
    qDebug() << " Thread running : " << workerThread->isRunning();
}

void Widget::countDone()
{
    qDebug() << "Widget, count Done";
    workerThread->exit();
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

void Widget::currentNumberString(NumberedString ns)
{
    ui->progressBar->setValue(ns.number());
    ui->infoLabel->setText(ns.string());
}
