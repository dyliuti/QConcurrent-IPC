#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFuture>
#include <QFutureWatcher>
#include <QTime>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    // 递归搜索文件.
    QStringList findFiles(const QString &startDir, const QStringList &filters);

    // 单线程
    QMap<QString, int> singleThreadedWordCount(const QStringList &files);

    // map function，计算一个文件中的单词数，由多个线程调用，必须线程安全。
    static QMap<QString, int> countWords(const QString &file);

    // reduce将map中的结果添加到最终结果。这个函子一次只能被一个线程调用。
    static void reduce(QMap<QString, int> &result, const QMap<QString, int> &w);

private slots:
    void on_chooseDirButton_clicked();
    void on_singleThreadedButton_clicked();
    void on_mapReducedButton_clicked();
    void on_findFilesButton_clicked();

private:
    Ui::Widget *ui;
    QString chosenDir;
    QStringList files;
    QFuture<QMap<QString, int>> future;
    QFutureWatcher<QMap<QString, int>> futureWatcher;
    QTime time;
};

#endif // WIDGET_H
