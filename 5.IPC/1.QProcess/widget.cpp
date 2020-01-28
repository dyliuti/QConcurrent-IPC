#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    mProcess = new QProcess(this);

    connect(mProcess,&QProcess::started,[](){
        qDebug() << "Process started";
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_chooseButton_clicked()
{
    processPath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                               "/home",
                                               tr("Programs (*.exe )"));
    if(!processPath.isNull()){
        ui->processPathLineEdit->setText(processPath);
    }
}

void Widget::on_startButton_clicked()
{
    if(!processPath.isNull()){
        // 有一些重载函数，如不单时进程路径，还有一些进程启动需要传入的参数
        mProcess->start(processPath);
    }
}

void Widget::on_stopButton_clicked()
{
    mProcess->close();
}

void Widget::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process finished, exit code : " << exitCode ;
    qDebug() << "Process finished, exit status : " << exitStatus ;
}
