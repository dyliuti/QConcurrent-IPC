#include "widget.h"
#include "ui_widget.h"
#include <QThread>
#include <QDebug>
#include <QtConcurrent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    qDebug() << "GUI Thread : " << QThread::currentThread();

    for( int i {0} ;  i < 30000 ; i++){
        list << i;
    }

    qDebug() << "Original value : " << list.last();
    connect(&futureWatcher, &QFutureWatcher<void>::started,[=](){
        qDebug() << "asynchronous : Work started.";
    });

    connect(&futureWatcher, &QFutureWatcher<void>::finished,[=](){
        qDebug() << "asynchronous : Work done.";
    });

    /* 当所观察的future报告进度时发出此信号，progressValue给出当前进度。为了避免GUI事件循环超载，QFutureWatcher限制了进程信号的发射速率。
     * 这意味着连接到此的槽可能无法获得将来生成的所有进度报告。最后一次进度更新(progressValue等于最大值)将始终被交付。
     */
    connect(&futureWatcher, &QFutureWatcher<void>::progressValueChanged,[=](int value){
        qDebug() << "Progress : " << value;
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::modify(int &value)
{
    qDebug() << "Modifying " << value << " result : " << value * 10
             << " Thread :" << QThread::currentThread();
    value = value * 10;
}

void Widget::on_modifyButton_clicked()
{
    /*
    //Lambda function
    auto modify = [](int &value){
            qDebug() << "Modifying " << value << " result : " << value * 10
                     << " Thread :" << QThread::currentThread();
            value = value * 10;
        };
    */

    // 在线程中的引用改变。std的shared_ptr也可以。但这个挺方便的
    /* 修改list中的值时是多线程异步进行的 */
    future = QtConcurrent::map(list, modify);
    futureWatcher.setFuture(future);
}

// 异步
void Widget::on_seeValuesButton_clicked()
{
    qDebug() << "Modified value : " << list.last();
    qDebug() << "List size : " << list.size();
//    for(int i=0; i<10; i++){
//        qDebug() << "list value : " << list.at(i);
//    }
}
