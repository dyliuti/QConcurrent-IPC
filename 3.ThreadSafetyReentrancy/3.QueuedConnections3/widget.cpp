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

    // 预先连接两个对象的函数，可确保槽中不丢失任何来自信号中的数据
    connect(producer, &Producer::data, consumer, &Consumer::data);
    // 这样就不需要重载QThread 子类的run函数，可以用其他类的函数作为线程执行的函数
    connect(thread, &QThread::started, producer, &Producer::startProduction);
    // 删除后想用只能再次初始化，并移到新新线程中去
    connect(thread, &QThread::finished, consumer, &QObject::deleteLater);

    // 生产者
    consumer->moveToThread(thread);

    thread->start();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    //Quit thread before killing app
    thread->quit();

    event->accept();
}
