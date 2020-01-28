#include "widget.h"
#include "ui_widget.h"
#include "worker.h"
#include <QDebug>
#include <QThread>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug() << "Ui thread : " << QThread::currentThread();

    thread = new QThread(this);


}

Widget::~Widget()
{
    delete ui;
}

/*
 * 注意，对象的所有活动计时器都将重置。计时器首先在当前线程中停止，然后在目标线程中重新启动(以相同的间隔)。
 * 因此，在线程之间不断地移动对象可能会无限期地延迟计时器事件。
 */
void Widget::on_startButton_clicked()
{
    Worker * worker = new Worker;

    worker->moveToThread(thread);

    connect(thread,&QThread::started,worker,&Worker::doWork);
    connect(thread,&QThread::finished,[](){
        qDebug() << "Thread finished";
    });
    connect(thread,&QThread::finished,worker,&Worker::deleteLater);

    thread->start();

}

void Widget::on_stopButton_clicked()
{
    thread->exit();
}

void Widget::on_infoButton_clicked()
{
    qDebug() << "Thread running : " << thread->isRunning();
}
