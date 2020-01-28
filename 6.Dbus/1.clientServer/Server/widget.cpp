#include "widget.h"
#include "ui_widget.h"
#include "calculatoradaptor.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 创建真实的计算器
    slaveCalculator = new SlaveCalculator(this);

    //Adaptor is used in the class exposing the service.
    //You have to pass the object you want to expose to DBus in the parameter
    //because methods will be called on it
    /* 在类中使用Adaptor，可将其暴露给服务。需将实例当作参数传给DBus */
    new CalculatorInterfaceAdaptor(slaveCalculator);

    QDBusConnection connection = QDBusConnection::sessionBus();

    //Here you pass in the object that you want expose to Dbus.
    //Take note of this info, it is used in client.
    /* 暴露给Dbus的类，这个信息是要在客户端中用的 */
    connection.registerObject("/CalcServicePath", slaveCalculator);

    //Here you pass in the service name.
    //Take note of this as it is also needed for clients to reach our exposed object.
    /* 服务名称，也是要在clients中用的。客户端通过这个获取服务进程中的类 */
    connection.registerService("com.blikoon.CalculatorService");
}

Widget::~Widget()
{
    delete ui;
}




