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

    qDebug() << "Ui thread : " << QThread::currentThread();

    connect(&watcher,&QFutureWatcher<void>::finished,[=](){
        qDebug() << "---------------------------Computation done----------------------";
        /* 防止wather.calcel()后程序崩溃，wather.calcel()后watcher是不可用的对象。 */
        if(!watcher.isCanceled()){
            QVector<int> resultVector = watcher.result();
            qDebug() << "Number of items in resultVector : " << resultVector.count();
            for( int i{0}; i < resultVector.count(); i++){
                qDebug() << " element [" << i << "] : " << resultVector[i];
            }
        }
        else{
            qDebug() << "Watcher already canceled ";
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

QFuture<QVector<int> > Widget::heavyWorkWithReturn()
{
    auto heavyWork = [=](){
        qDebug() << "Heavy work running in thread : " << QThread::currentThread();
        QVector<int> intVector;

        for(int i{0} ; i < 1000000001 ; i++){
            if((i%100000) == 0){
                double percentage = ((i/1000000000.0)) * 100;
                qDebug() << "Heavy work with return : " << QVariant::fromValue(percentage).toInt() ;
                intVector.push_back(QVariant::fromValue(percentage).toInt());
            }
        }
        return  intVector;
    };

    return  QtConcurrent::run(heavyWork);
}

void Widget::on_startButton_clicked()
{
    future =  heavyWorkWithReturn();
    watcher.setFuture(future);
}

void Widget::on_cancelButton_clicked()
{
    watcher.cancel();
}

/* 没效果，不好用，用完后不做异常校验（!watcher.isCanceled()）还有副作用 */
void Widget::on_pauseButton_clicked()
{
    watcher.setPaused(true);
    //watcher.pause(); // 里面调用了setPaused(true)
}

void Widget::on_resumeButton_clicked()
{
    watcher.resume();
}
