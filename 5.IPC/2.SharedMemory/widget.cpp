#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QBuffer>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    sharedMemory("SharedMemoryDemoApp")
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_loadImageButton_clicked()
{
    // 如果该进程附加到共享内存段，则返回true
    if (sharedMemory.isAttached()){
        qDebug() << "Memory is attached, detaching";
        detach();
    }

    ui->imageLabel->setText(tr("Select an image file"));
    QString fileName = QFileDialog::getOpenFileName(nullptr, QString(), QString(),
                                                    tr("Images (*.png *.xpm *.jpg)"));
    QImage image;
    if (!image.load(fileName)) {
        ui->imageLabel->setText(tr("Selected file is not an image, please select another."));
        return;
    }

    ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    /* 写入到共享内存 */
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << image;
    int size =  buffer.size();

    qDebug() << "Size : " << size;
    if (!sharedMemory.create(size)) {
        ui->imageLabel->setText(tr("Unable to create shared memory segment."));
        qDebug() << sharedMemory.errorString();
        qDebug()  << "Is attached : " << sharedMemory.isAttached();
        return;
    }

    /* 写入到共享区数据区时需要加锁保护 */
    sharedMemory.lock();
    char *to = (char*)sharedMemory.data();
    // QBuffer -> QByteArray -> char * / const char *
    const char *from = buffer.data().data();
    // qMin做异常保护
    memcpy(to, from, qMin(sharedMemory.size(), size));
    sharedMemory.unlock();
}

void Widget::on_loadShareMemoryButton_clicked()
{
    // 尝试将进程附加到共享内存段
    if (!sharedMemory.attach()) {
        ui->imageLabel->setText(tr("Unable to attach to shared memory segment.\n" \
                                   "Load in the data first."));
        return;
    }

    QBuffer buffer;
    QDataStream in(&buffer);
    QImage image;

    // 从共享数据区读取数据，并写入进程中的内存QImage中
    sharedMemory.lock();
    buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
    buffer.open(QBuffer::ReadOnly);
    in >> image;
    sharedMemory.unlock();

    // 从共享内存段中分离进程。如果这是附加到共享内存段的最后一个进程，则系统释放共享内存段，即内容被销毁。
    // deatch失败要不是没attached成功，要不就是被另一个进程Locked的了。
    sharedMemory.detach();

    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}

void Widget::detach()
{
    if(!sharedMemory.detach()){
        ui->imageLabel->setText("Can not detach from the shared memory");
    }
}
