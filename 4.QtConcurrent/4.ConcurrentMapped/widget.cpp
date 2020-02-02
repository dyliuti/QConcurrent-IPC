#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QThread>
#include <QtConcurrent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    qDebug() << "GUI Thread : " << QThread::currentThread();

    for(int i {0} ;  i < 30000 ; i++){
        list << i;
    }

    connect(&futureWatcher, &QFutureWatcher<void>::started,[=](){
        qDebug() << "asynchronous : Work started.";

    });

    connect(&futureWatcher, &QFutureWatcher<void>::finished,[=](){
        qDebug() << "asynchronous : Work finished.";
        qDebug() << "Result count : " << future.resultCount();

        //qDebug() << future.results();
        for( int i{0} ; i < future.resultCount(); i ++){
            qDebug() << "Result " << i << " :" << future.resultAt(i);
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

int Widget::modify(const int &value)
{
    qDebug() << "Modifying " << value << " result : " << value * 10
             << " Thread :" << QThread::currentThread();
    return value * 10;
}

void Widget::on_modifyButton_clicked()
{
    /*
    std::function<int(const int &)> modify = [](const int & value){
        qDebug() << "Modifying " << value << " result : " << value * 10
                 << " Thread :" << QThread::currentThread();
        return value * 10;
    };
    */

    /* 修改list中的值时是异步的 */
    future = QtConcurrent::mapped(list, modify);
    futureWatcher.setFuture(future);
    /* 同步设置 */
//    future.waitForFinished();

//    qDebug() << "asynchronous : Work finished.";
//    qDebug() << "Result count : " << future.resultCount();

//    //qDebug() << future.results();

//    for( int i{0} ; i < future.resultCount(); i ++){
//        qDebug() << "Result " << i << " :" << future.resultAt(i);
//    }
}

/* 若future设置了同步，UI中的点击操作只有当future执行完后才能执行 */
void Widget::on_seeValuesButton_clicked()
{
    qDebug() << "Modified value : " << list.last();
}
