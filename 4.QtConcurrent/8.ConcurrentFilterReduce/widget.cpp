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
    qDebug() << "Ui thread : " <<QThread::currentThread();

    for(int i{0} ; i < 100 ; i++){
        intList << QRandomGenerator::global()->bounded(1000);
    }

    qDebug() << "Before , item count : " << intList.count();

    ui->originalList->clear();
    ui->filteredList->clear();

    // 显示原始列表中的值
    foreach (int value, intList) {
        ui->originalList->addItem(QString::number(value));
    }

    filterValue = ui->filterSpinBox->value();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::reduce(int &sum, int value)
{
    sum  += value;
}

void Widget::on_filterButton_clicked()
{
    auto filter = [=](const int value){
        if(value >= filterValue){
            qDebug() << "Thread " << QThread::currentThread() << ". Value " << value << " greater that threshold " <<filterValue << " filtering out.";
            return false;
        }
        return true;
    };

    ui->filteredList->clear();

    future = QtConcurrent::filtered(intList,filter);
    future.waitForFinished();

    qDebug() << "synchronous : Work finished.";

    // 获取过滤后的结果
    QList<int> newList = future.results();

    qDebug() << "After , item count : " << newList.count();

    foreach (int value, newList) {
        ui->filteredList->addItem(QString::number(value));
    }

    QFuture <int> futureReduce = QtConcurrent::filteredReduced(intList, filter, reduce);
    futureReduce.waitForFinished();
    qDebug() << "The sum is : " << futureReduce.result();
    ui->sumLabel->setText(QString::number(futureReduce.result()));
}

void Widget::on_filterSpinBox_valueChanged(int arg1)
{
    filterValue = arg1;
}
