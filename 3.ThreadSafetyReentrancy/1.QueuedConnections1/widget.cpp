#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QApplication>
#include <QCloseEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug() << "Ui thread : " << QThread::currentThread();

    producer = new Producer(this);
    ui->verticalLayout->addWidget(producer);
    consumer = new Consumer();
    thread = new QThread(this);

    connect(thread,&QThread::finished,[=](){
        qDebug() << "Application about to be killed , thread : " << QThread::currentThread();
        QApplication::quit();
    });

    // queued connection
    connect(producer, &Producer::data, consumer, &Consumer::data);
    // Producer在主线程中有事件循环
    // 自动链接的类型，调用函数Queue还是直连之前会判断发送者与接收者是否是在同一个线程的。所以不影响信号槽的链接。
    consumer->moveToThread(thread);

    thread->start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    //停掉app前停止线程
    thread->quit();
    event->accept();
}
