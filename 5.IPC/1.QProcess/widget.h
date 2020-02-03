#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_chooseButton_clicked();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void  finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QString processPath;
    QProcess * mProcess;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
