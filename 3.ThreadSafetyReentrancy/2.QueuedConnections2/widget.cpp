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
    thread = new Thread(producer);

    connect(thread, &QThread::finished, [=](){
        qDebug() << "Application about to be killed , thread : " << QThread::currentThread();
        QApplication::quit();
    });

    thread->start();
    // 在链接前就进行生产，是有问题的，连接前，这些数据将会丢失
    producer->startProduction();
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
